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
#include <stdarg.h>

//
// Public
// 
#include "Indicium/Engine/IndiciumCore.h"
#include "Indicium/Engine/IndiciumDirect3D9.h"
#include "Indicium/Engine/IndiciumDirect3D10.h"
#include "Indicium/Engine/IndiciumDirect3D11.h"
#include "Indicium/Engine/IndiciumDirect3D12.h"

//
// Internal
// 
#include "Engine.h"
#include "Game/Game.h"
#include "Global.h"

//
// Logging
// 
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>


INDICIUM_API PINDICIUM_ENGINE IndiciumEngineAlloc()
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
    // Set up logging
    //
    auto logger = spdlog::basic_logger_mt(
        "indicium",
        Indicium::Core::Util::expand_environment_variables("%TEMP%\\Indicium-Supra.log")
        );

#if _DEBUG
    spdlog::set_level(spdlog::level::debug);
    logger->flush_on(spdlog::level::debug);
#else
    logger->flush_on(spdlog::level::info);
#endif

    spdlog::set_default_logger(logger);

    logger = spdlog::get("indicium")->clone("api");
    
    logger->info("Indicium engine initialized, attempting to launch main thread");
   
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
        logger->error("Could not create main thread, library unusable");
        return INDICIUM_ERROR_CREATE_THREAD_FAILED;
    }

    logger->info("Main thread created successfully");

    return INDICIUM_ERROR_NONE;
}

INDICIUM_API VOID IndiciumEngineShutdown(PINDICIUM_ENGINE Engine, PFN_INDICIUM_GAME_UNHOOKED EvtIndiciumGameUnhooked)
{
    if (!Engine) {
        return;
    }

    auto logger = spdlog::get("indicium")->clone("api");

    logger->info("Indicium engine shutdown requested, attempting to terminate main thread");

    const auto ret = SetEvent(Engine->EngineCancellationEvent);

    if (!ret)
    {
        logger->error("SetEvent failed: {}", GetLastError());
    }

    const auto result = WaitForSingleObject(Engine->EngineThread, 3000);

    switch (result)
    {
    case WAIT_ABANDONED:
        logger->error("Unknown state, host process might crash");
        break;
    case WAIT_OBJECT_0:
        logger->info("Hooks removed, notifying caller");
        break;
    case WAIT_TIMEOUT:
        TerminateThread(Engine->EngineThread, 0);
        logger->error("Thread hasn't finished clean-up within expected time, terminating");
        break;
    case WAIT_FAILED:
        logger->error("Unknown error, host process might crash");
        break;
    default:
        TerminateThread(Engine->EngineThread, 0);
        logger->error("Unexpected return value, terminating");
        break;
    }

    if (EvtIndiciumGameUnhooked) {
        EvtIndiciumGameUnhooked();
    }

    CloseHandle(Engine->EngineCancellationEvent);
    CloseHandle(Engine->EngineThread);

    logger->info("Engine shutdown complete");
    logger->flush();
}

INDICIUM_API VOID IndiciumEngineFree(PINDICIUM_ENGINE Engine)
{
    if (!Engine) {
        return;
    }

    free(Engine);
}

#ifndef INDICIUM_NO_D3D9

INDICIUM_API VOID IndiciumEngineSetD3D9EventCallbacks(PINDICIUM_ENGINE Engine, PINDICIUM_D3D9_EVENT_CALLBACKS Callbacks)
{
    if (Engine) {
        Engine->EventsD3D9 = *Callbacks;
    }
}

#endif

#ifndef INDICIUM_NO_D3D10

INDICIUM_API VOID IndiciumEngineSetD3D10EventCallbacks(PINDICIUM_ENGINE Engine, PINDICIUM_D3D10_EVENT_CALLBACKS Callbacks)
{
    if (Engine) {
        Engine->EventsD3D10 = *Callbacks;
    }
}

#endif

#ifndef INDICIUM_NO_D3D11

INDICIUM_API VOID IndiciumEngineSetD3D11EventCallbacks(PINDICIUM_ENGINE Engine, PINDICIUM_D3D11_EVENT_CALLBACKS Callbacks)
{
    if (Engine) {
        Engine->EventsD3D11 = *Callbacks;
    }
}

#endif

#ifndef INDICIUM_NO_D3D12

INDICIUM_API VOID IndiciumEngineSetD3D12EventCallbacks(PINDICIUM_ENGINE Engine, PINDICIUM_D3D12_EVENT_CALLBACKS Callbacks)
{
    if (Engine) {
        Engine->EventsD3D12 = *Callbacks;
    }
}

#endif

INDICIUM_API VOID IndiciumEngineLogDebug(LPCSTR Format, ...)
{
    auto logger = spdlog::get("indicium")->clone("host");
    va_list args;
    char buf[1000]; // TODO: dumb, make better
    va_start(args, Format);
    vsnprintf(buf, sizeof(buf), Format, args);
    va_end(args);
    logger->debug(buf);
}

INDICIUM_API VOID IndiciumEngineLogInfo(LPCSTR Format, ...)
{
    auto logger = spdlog::get("indicium")->clone("host");
    va_list args;
    char buf[1000]; // TODO: dumb, make better
    va_start(args, Format);
    vsnprintf(buf, sizeof(buf), Format, args);
    va_end(args);
    logger->info(buf);
}

INDICIUM_API VOID IndiciumEngineLogWarning(LPCSTR Format, ...)
{
    auto logger = spdlog::get("indicium")->clone("host");
    va_list args;
    char buf[1000]; // TODO: dumb, make better
    va_start(args, Format);
    vsnprintf(buf, sizeof(buf), Format, args);
    va_end(args);
    logger->warn(buf);
}

INDICIUM_API VOID IndiciumEngineLogError(LPCSTR Format, ...)
{
    auto logger = spdlog::get("indicium")->clone("host");
    va_list args;
    char buf[1000]; // TODO: dumb, make better
    va_start(args, Format);
    vsnprintf(buf, sizeof(buf), Format, args);
    va_end(args);
    logger->error(buf);
}
