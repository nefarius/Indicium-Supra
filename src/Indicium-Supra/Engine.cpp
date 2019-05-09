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


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Indicium/Engine/IndiciumCore.h"
#include "Indicium/Engine/IndiciumDirect3D9.h"
#include "Indicium/Engine/IndiciumDirect3D10.h"
#include "Indicium/Engine/IndiciumDirect3D11.h"
#include "Indicium/Engine/IndiciumDirect3D12.h"

#define POCO_NO_UNWINDOWS

#include "Engine.h"
#include "Game/Game.h"
#include "Global.h"

#include <Poco/AutoPtr.h>
#include <Poco/Util/IniFileConfiguration.h>
#include <Poco/Message.h>
#include <Poco/Logger.h>
#include <Poco/FileChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/Path.h>

using Poco::AutoPtr;
using Poco::Util::IniFileConfiguration;
using Poco::Message;
using Poco::Logger;
using Poco::FileChannel;
using Poco::PatternFormatter;
using Poco::FormattingChannel;
using Poco::Path;


INDICIUM_API PINDICIUM_ENGINE IndiciumEngineAlloc(void)
{
    const auto engine = static_cast<PINDICIUM_ENGINE>(malloc(sizeof(INDICIUM_ENGINE)));

    if (!engine) {
        return nullptr;
    }

    ZeroMemory(engine, sizeof(INDICIUM_ENGINE));

    return engine;
}

INDICIUM_API INDICIUM_ERROR IndiciumEngineInit(PINDICIUM_ENGINE Engine, PFN_INDICIUM_GAME_HOOKED EvtIndiciumGameHooked)
{
    if (!Engine) {
        return INDICIUM_ERROR_INVALID_ENGINE_HANDLE;
    }

    //
    // Callback invoked when initialization finished
    // 
    Engine->EvtIndiciumGameHooked = EvtIndiciumGameHooked;

    //
    // Event to notify engine thread about termination
    // 
    Engine->EngineCancellationEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    //
    // Event to notify shutdown function that the game has been unhooked
    // 
    Engine->EngineCancellationCompletedEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    //
    // Fetch configuration
    // 
    Path cfgFile(GlobalState::instance().root_path(), "Indicium-Supra.ini");
    Engine->Configuration = new IniFileConfiguration();

    //
    // Set up logging
    // 
    AutoPtr<FileChannel> pFileChannel(new FileChannel);
    pFileChannel->setProperty("rotateOnOpen", "true");
    pFileChannel->setProperty("archive", "timestamp");
    pFileChannel->setProperty(
        "path",
        Path::expand(Engine->Configuration->getString(
            "global.logFile",
            "%TEMP%\\Indicium-Supra.log")));

    AutoPtr<PatternFormatter> pPF(new PatternFormatter);
    pPF->setProperty("pattern", "%Y-%m-%d %H:%M:%S.%i %s [%p]: %t");
    AutoPtr<FormattingChannel> pFC(new FormattingChannel(pPF, pFileChannel));

    Logger::root().setChannel(pFC);

    auto& logger = Logger::get(__func__);

    logger.information("Indicium engine initialized, attempting to launch main thread");

    //
    // Kickstart hooking the rendering pipeline
    // 
    Engine->EngineThread = CreateThread(
        nullptr,
        0,
        reinterpret_cast<LPTHREAD_START_ROUTINE>(IndiciumMainThread),
        Engine,
        0,
        nullptr
    );

    if (!Engine->EngineThread) {
        logger.fatal("Couldn't create main thread, library unusable");
        return INDICIUM_ERROR_CREATE_THREAD_FAILED;
    }

    logger.information("Main thread created successfully");

    return INDICIUM_ERROR_NONE;
}

INDICIUM_API VOID IndiciumEngineShutdown(PINDICIUM_ENGINE Engine, PFN_INDICIUM_GAME_UNHOOKED EvtIndiciumGameUnhooked)
{
    if (!Engine) {
        return;
    }

    auto& logger = Logger::get(__func__);

    logger.information("Indicium engine shutdown requested, attempting to terminate main thread");

    SetEvent(Engine->EngineCancellationEvent);
    const auto result = WaitForSingleObject(Engine->EngineCancellationCompletedEvent, 1000);

    switch (result)
    {
    case WAIT_ABANDONED:
        logger.error("Unknown state, host process might crash");
        break;
    case WAIT_OBJECT_0:
        logger.information("Hooks removed, notifying caller");
        break;
    case WAIT_TIMEOUT:
        logger.error("Thread hasn't finished clean-up within expected time");
        break;
    case WAIT_FAILED:
        logger.error("Unknown error, host process might crash");
        break;
    default:
        logger.warning("Unexpected return value");
        break;
    }        

    if (EvtIndiciumGameUnhooked) {
        EvtIndiciumGameUnhooked();
    }

    CloseHandle(Engine->EngineCancellationEvent);
    CloseHandle(Engine->EngineCancellationCompletedEvent);
    CloseHandle(Engine->EngineThread);

    logger.information("Engine shutdown complete");

    Logger::shutdown();
}

INDICIUM_API VOID IndiciumEngineFree(PINDICIUM_ENGINE Engine)
{
    if (!Engine) {
        return;
    }

    free(Engine);
}

INDICIUM_API VOID IndiciumEngineSetD3D9EventCallbacks(PINDICIUM_ENGINE Engine, PINDICIUM_D3D9_EVENT_CALLBACKS Callbacks)
{
    if (Engine) {
        Engine->EventsD3D9 = *Callbacks;
    }
}

INDICIUM_API VOID IndiciumEngineSetD3D10EventCallbacks(PINDICIUM_ENGINE Engine, PINDICIUM_D3D10_EVENT_CALLBACKS Callbacks)
{
    if (Engine) {
        Engine->EventsD3D10 = *Callbacks;
    }
}

INDICIUM_API VOID IndiciumEngineSetD3D11EventCallbacks(PINDICIUM_ENGINE Engine, PINDICIUM_D3D11_EVENT_CALLBACKS Callbacks)
{
    if (Engine) {
        Engine->EventsD3D11 = *Callbacks;
    }
}

INDICIUM_API VOID IndiciumEngineSetD3D12EventCallbacks(PINDICIUM_ENGINE Engine, PINDICIUM_D3D12_EVENT_CALLBACKS Callbacks)
{
    if (Engine) {
        Engine->EventsD3D12 = *Callbacks;
    }
}
