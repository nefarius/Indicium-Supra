#include "Agrippa.h"
#include "../../include/IndiciumPlugin.h"
#include <mutex>
#include "IPCServer.h"

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

    std::call_once(initFlag, []()
    {
        auto& logger = Logger::get("Present");

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
            delete g_ipcServer;
        }
    });

    AutoPtr<Notification> pPBN(g_ipcQueue.dequeueNotification());

    if (!pPBN.isNull())
    {
        logger.information("Message received");
        auto pBuffer = dynamic_cast<PixelBufferNotification*>(pPBN.get());

        logger.information("%s x %s", std::to_string(pBuffer->getWidth()), std::to_string(pBuffer->getHeight()));
    }
}
