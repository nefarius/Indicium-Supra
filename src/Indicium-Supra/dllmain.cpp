/*
MIT License

Copyright (c) 2018 Benjamin Höglinger

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "dllmain.h"
#include "Global.h"

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
#include <Poco/Util/IniFileConfiguration.h>

using Poco::Message;
using Poco::Logger;
using Poco::FileChannel;
using Poco::AutoPtr;
using Poco::PatternFormatter;
using Poco::FormattingChannel;
using Poco::Path;
using Poco::Util::IniFileConfiguration;

HANDLE g_hDllHandle = nullptr;
std::once_flag flag;
extern AutoPtr<IniFileConfiguration> g_config;


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
    g_hDllHandle = hInstance;

    DisableThreadLibraryCalls(static_cast<HMODULE>(hInstance));

    //
    // Fetch configuration
    // 
    Path cfgFile(GlobalState::instance().rootPath(), "Indicium-Supra.ini");
    g_config = new IniFileConfiguration();
    
    //
    // Set up logging
    // 
    AutoPtr<FileChannel> pFileChannel(new FileChannel);
    pFileChannel->setProperty("path", Path::expand(g_config->getString("global.logFile", "%TEMP%\\Indicium-Supra.log")));
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

