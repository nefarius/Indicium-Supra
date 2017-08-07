#include "dllmain.h"

#include <MinHook.h>
#include <Utils/Windows.h>
#include <Game/Game.h>

// STL
#include <thread>
#include <mutex>

// POCO
#include <Poco/Message.h>
#include <Poco/Logger.h>
#include <Poco/FileChannel.h>
#include <Poco/AutoPtr.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/Path.h>

using Poco::Message;
using Poco::Logger;
using Poco::FileChannel;
using Poco::AutoPtr;
using Poco::PatternFormatter;
using Poco::FormattingChannel;
using Poco::Path;

HANDLE g_hDllHandle = nullptr;
std::once_flag flag;


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
    g_hDllHandle = hInstance;

    DisableThreadLibraryCalls(static_cast<HMODULE>(hInstance));

    std::string logfile("%TEMP%\\Indicium-Supra.log");

    AutoPtr<FileChannel> pFileChannel(new FileChannel);
    pFileChannel->setProperty("path", Path::expand(logfile));
    AutoPtr<PatternFormatter> pPF(new PatternFormatter);
    pPF->setProperty("pattern", "%Y-%m-%d %H:%M:%S.%i %s [%p]: %t");
    AutoPtr<FormattingChannel> pFC(new FormattingChannel(pPF, pFileChannel));

    // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
    {
        Logger::root().setChannel(pFC);

        auto& logger = Logger::get(__func__);

        logger.information("Library loaded, attempting to launch main thread");

        std::call_once(flag, []()
        {
            auto& logger = Logger::get(__func__);
            auto hMain = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(initGame), nullptr, 0, nullptr);

            if (hMain == nullptr)
            {
                logger.fatal("Couldn't create main thread, library unusable");
            }
            else
            {
                logger.information("Main thread created successfully");
            }
        });

        return TRUE;
    }
    case DLL_PROCESS_DETACH:
    {
        auto& logger = Logger::get(__func__);

        logger.information("Library unloading");

        break;
    }
    };

    return TRUE;
}

