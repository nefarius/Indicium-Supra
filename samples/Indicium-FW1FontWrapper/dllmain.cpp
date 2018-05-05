#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "FW1FontWrapper.h"

#include <Indicium\Plugin\Direct3D11.h>

#ifdef _M_IX86
#pragma comment(lib, "x86\\FW1FontWrapper.lib")
#else
#pragma comment(lib, "x64\\FW1FontWrapper.lib")
#endif
#pragma comment(lib, "dxguid.lib")


/**
 * \fn  BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
 *
 * \brief   Your typical DLL entry point function. We're not doing much here since a special
 *          initialization routine get's called upon getting loaded by Indicium-Supra.
 *
 * \author  Benjamin "Nefarius" Höglinger
 * \date    05.05.2018
 *
 * \param   hInstance   The instance.
 * \param   dwReason    The reason.
 * \param   parameter3  The third parameter.
 *
 * \return  A WINAPI.
 */
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
    //
    // We don't need to get notified in thread attach- or detachments
    // 
    DisableThreadLibraryCalls(static_cast<HMODULE>(hInstance));

    return TRUE;
}

INDICIUM_EXPORT(BOOLEAN) indicium_plugin_init(Direct3DVersion version)
{
    return (version == Direct3DVersion::Direct3D11);
}

INDICIUM_EXPORT(VOID) indicium_plugin_d3d11_present(
    IDXGISwapChain  *pSwapChain,
    UINT            SyncInterval,
    UINT            Flags
)
{
    static ID3D11DeviceContext* ctx = nullptr;
    static ID3D11Device* dev = nullptr;

    D3D11_DEVICE_CONTEXT_FROM_SWAPCHAIN(pSwapChain, &dev, &ctx);

    static IFW1Factory *pFW1Factory = nullptr;
    static HRESULT hResult;
    if (!pFW1Factory)
        hResult = FW1CreateFactory(FW1_VERSION, &pFW1Factory);

    static IFW1FontWrapper *pFontWrapper = nullptr;
    if (!pFontWrapper)
        hResult = pFW1Factory->CreateFontWrapper(dev, L"Arial", &pFontWrapper);

    static auto x = 1280.0f;

    //
    // Very crude marquee mechanism for demonstration
    // 
    if (x <= -1280.0f)x = 1280.0f;

    pFontWrapper->DrawString(
        ctx,
        L"Injected via Indicium-Supra by Nefarius",// String
        50.0f,// Font size
        x--,// X position
        30.0f,// Y position
        0xff0099ff,// Text color, 0xAaBbGgRr
        FW1_RESTORESTATE// Flags (for example FW1_RESTORESTATE to keep context states unchanged)
    );

    pFontWrapper->Release();
    pFW1Factory->Release();
}

