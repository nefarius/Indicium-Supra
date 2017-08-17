#include "Agrippa.h"
#include "../../include/IndiciumPlugin.h"
#include <mutex>
#include "IPCServer.h"
#include <dxgi.h>
#include <d3d11.h>

#include <Poco/AutoPtr.h>
#include <Poco/SharedPtr.h>
#include <Poco/ThreadPool.h>
#include <Poco/Notification.h>
#include <Poco/NotificationQueue.h>
#include <Poco/Logger.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/Path.h>
#include <Poco/FileChannel.h>
#include "PixelBufferNotification.h"

using Poco::AutoPtr;
using Poco::SharedPtr;
using Poco::ThreadPool;
using Poco::Notification;
using Poco::NotificationQueue;
using Poco::FileChannel;
using Poco::Path;
using Poco::PatternFormatter;
using Poco::FormattingChannel;
using Poco::Logger;


static std::once_flag initFlag;
SharedPtr<IPCServer> g_ipcServer;
static NotificationQueue g_ipcQueue;


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved
)
{
    DisableThreadLibraryCalls(static_cast<HMODULE>(hModule));

    if (ul_reason_for_call != DLL_PROCESS_ATTACH)
        return TRUE;

    std::string logfile("%TEMP%\\Indicium-Agrippa.Plugin.log");

    AutoPtr<FileChannel> pFileChannel(new FileChannel);
    pFileChannel->setProperty("path", Path::expand(logfile));
    AutoPtr<PatternFormatter> pPF(new PatternFormatter);
    pPF->setProperty("pattern", "%Y-%m-%d %H:%M:%S.%i %s [%p]: %t");
    AutoPtr<FormattingChannel> pFC(new FormattingChannel(pPF, pFileChannel));

    Logger::root().setChannel(pFC);

    return TRUE;
}

INDICIUM_EXPORT Present(IID guid, LPVOID unknown, Direct3DVersion version)
{
    static auto& logger = Logger::get(__func__);
    static IDXGISwapChain* sc = nullptr;
    static ID3D11Device* dev = nullptr;
    static ID3D11DeviceContext* ctx = nullptr;
    static ID3D11Texture2D* pBackBuffer = nullptr;
    static ID3D11Texture2D *surfaceRGBA = nullptr;

    std::call_once(initFlag, [&]()
    {
        logger.information("Initializing library");

        g_ipcServer = new IPCServer(g_ipcQueue);

        try
        {
            logger.information("Launching IPCServer");

            ThreadPool::defaultPool().start(*g_ipcServer);

            logger.information("IPCServer running");
        }
        catch (Poco::NoThreadAvailableException)
        {
            logger.fatal("Couldn't launch IPCServer");
            delete g_ipcServer;
        }

        if (IS_DIRECT3D11(version))
        {
            sc = static_cast<IDXGISwapChain*>(unknown);
            sc->GetDevice(__uuidof(dev), reinterpret_cast<void**>(&dev));

            dev->GetImmediateContext(&ctx);

            auto hr = sc->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
            if (FAILED(hr))
            {
                logger.error("Couldn't get swapchain back buffer");
                return;
            }

            D3D11_TEXTURE2D_DESC desc;
            pBackBuffer->GetDesc(&desc);

            logger.information("Back buffer format: %s", std::to_string(desc.Format));
        }
    });

    static int x, y;

    if (surfaceRGBA)
    {
        ctx->CopySubresourceRegion(pBackBuffer, 0, x, y, 0, surfaceRGBA, 0, nullptr);
    }

    AutoPtr<Notification> pPBN(g_ipcQueue.dequeueNotification());

    if (!pPBN.isNull())
    {
        logger.information("Message received");
        auto pBuffer = dynamic_cast<PixelBufferNotification*>(pPBN.get());

        logger.information("RowPitch: %s", std::to_string(pBuffer->getWidth() * sizeof(RGBQUAD)));

        if (IS_DIRECT3D11(version))
        {
            logger.information("In D3D11");

            D3D11_TEXTURE2D_DESC desc_rgba;

            desc_rgba.Width = pBuffer->getWidth();
            desc_rgba.Height = pBuffer->getHeight();
            desc_rgba.MipLevels = 1;
            desc_rgba.ArraySize = 1;
            desc_rgba.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc_rgba.SampleDesc.Count = 1;
            desc_rgba.SampleDesc.Quality = 0;
            desc_rgba.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            desc_rgba.Usage = D3D11_USAGE_DYNAMIC;
            desc_rgba.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc_rgba.MiscFlags = 0;

            D3D11_SUBRESOURCE_DATA subresource;
            subresource.pSysMem = pBuffer->getBuffer();
            subresource.SysMemPitch = (pBuffer->getWidth() * sizeof(RGBQUAD)) * sizeof(unsigned char);

            if (surfaceRGBA)
                surfaceRGBA->Release();

            auto hr = dev->CreateTexture2D(&desc_rgba, &subresource, &surfaceRGBA);
            if (FAILED(hr))
            {
                logger.error("Can't create DX texture");
            }

            x = pBuffer->getX();
            y = pBuffer->getY();

            pBuffer->duplicate();
        }
    }
}
