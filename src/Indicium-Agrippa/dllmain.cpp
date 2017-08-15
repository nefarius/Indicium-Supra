#include "Agrippa.h"
#include "../../include/IndiciumPlugin.h"
#include <mutex>
#include "IPCServer.h"

#include <Poco/AutoPtr.h>
#include <Poco/SharedPtr.h>
#include <Poco/ThreadPool.h>
#include <Poco/Notification.h>
#include <Poco/NotificationQueue.h>

using Poco::AutoPtr;
using Poco::SharedPtr;
using Poco::ThreadPool;
using Poco::Notification;
using Poco::NotificationQueue;

static std::once_flag initFlag;
SharedPtr<IPCServer> g_ipcServer;
static NotificationQueue g_ipcQueue;


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

INDICIUM_EXPORT Present(IID guid, LPVOID unknown, Direct3DVersion version)
{
    std::call_once(initFlag, []()
    {
        g_ipcServer = new IPCServer(g_ipcQueue);

        try
        {
            ThreadPool::defaultPool().start(*g_ipcServer);
        }
        catch (Poco::NoThreadAvailableException)
        {
            delete g_ipcServer;
        }
    });
}
