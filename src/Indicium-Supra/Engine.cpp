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
#include "Indicium/Engine/IndiciumDXGI.h"
#include "Indicium/Engine/IndiciumDirect3D9.h"
#include "Indicium/Engine/IndiciumDirect3D10.h"
#include "Indicium/Engine/IndiciumDirect3D11.h"
#include "Indicium/Engine/IndiciumDirect3D12.h"
#include "Indicium/Engine/IndiciumCoreAudio.h"

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

//
// STL
// 
#include <map>

//
// Keep track of HINSTANCE/HANDLE to engine handle association
// 
static std::map<HMODULE, PINDICIUM_ENGINE> g_EngineHostInstances;


INDICIUM_API INDICIUM_ERROR IndiciumEngineCreate(HMODULE HostInstance, PINDICIUM_ENGINE_CONFIG EngineConfig, PINDICIUM_ENGINE * Engine)
{
    //
    // Check if we got initialized for this instance before
    // 
    if (g_EngineHostInstances.count(HostInstance)) {
        return INDICIUM_ERROR_ENGINE_ALREADY_ALLOCATED;
    }

    //
    // Increase host DLL reference count
    // 
    HMODULE hmod;
    if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
        reinterpret_cast<LPCTSTR>(HostInstance),
        &hmod)) {
        return INDICIUM_ERROR_REFERENCE_INCREMENT_FAILED;
    }

    const auto engine = static_cast<PINDICIUM_ENGINE>(malloc(sizeof(INDICIUM_ENGINE)));

    if (!engine) {
        return INDICIUM_ERROR_ENGINE_ALLOCATION_FAILED;
    }

    ZeroMemory(engine, sizeof(INDICIUM_ENGINE));
    engine->HostInstance = HostInstance;
    CopyMemory(&engine->EngineConfig, EngineConfig, sizeof(INDICIUM_ENGINE_CONFIG));

    //
    // Event to notify engine thread about termination
    // 
    engine->EngineCancellationEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    //
    // Set up logging
    //
    auto logger = spdlog::basic_logger_mt(
        "indicium",
        Indicium::Core::Util::expand_environment_variables(EngineConfig->Logging.FilePath)
    );

#if _DEBUG
    spdlog::set_level(spdlog::level::debug);
    logger->flush_on(spdlog::level::debug);
#else
    logger->flush_on(spdlog::level::info);
#endif

    if (EngineConfig->Logging.IsEnabled) {
        spdlog::set_default_logger(logger);
    }

    logger = spdlog::get("indicium")->clone("api");

    logger->info("Indicium engine initialized, attempting to launch main thread");

    //
    // Kickstart hooking the rendering pipeline
    // 
    engine->EngineThread = CreateThread(
        nullptr,
        0,
        reinterpret_cast<LPTHREAD_START_ROUTINE>(IndiciumMainThread),
        engine,
        0,
        nullptr
    );

    if (!engine->EngineThread) {
        logger->error("Could not create main thread, library unusable");
        return INDICIUM_ERROR_CREATE_THREAD_FAILED;
    }

    logger->info("Main thread created successfully");

    if (Engine)
    {
        *Engine = engine;
    }

    g_EngineHostInstances[HostInstance] = engine;

    return INDICIUM_ERROR_NONE;
}

INDICIUM_API INDICIUM_ERROR IndiciumEngineDestroy(HMODULE HostInstance)
{
    if (!g_EngineHostInstances.count(HostInstance)) {
        return INDICIUM_ERROR_INVALID_HMODULE_HANDLE;
    }

    const auto& engine = g_EngineHostInstances[HostInstance];
    auto logger = spdlog::get("indicium")->clone("api");

    logger->info("Freeing remaining resources");

    CloseHandle(engine->EngineCancellationEvent);
    CloseHandle(engine->EngineThread);

    const auto it = g_EngineHostInstances.find(HostInstance);
    g_EngineHostInstances.erase(it);

    logger->info("Engine shutdown complete");
    logger->flush();

    return INDICIUM_ERROR_NONE;
}

INDICIUM_API INDICIUM_ERROR IndiciumEngineAllocCustomContext(PINDICIUM_ENGINE Engine, PVOID* Context, size_t ContextSize)
{
    if (!Engine) {
        return INDICIUM_ERROR_INVALID_ENGINE_HANDLE;
    }

    if (Engine->CustomContext) {
        IndiciumEngineFreeCustomContext(Engine);
    }

    Engine->CustomContext = malloc(ContextSize);

    if (!Engine->CustomContext) {
        return INDICIUM_ERROR_CONTEXT_ALLOCATION_FAILED;
    }

    ZeroMemory(Engine->CustomContext, ContextSize);
    *Context = Engine->CustomContext;

    return INDICIUM_ERROR_NONE;
}

INDICIUM_API INDICIUM_ERROR IndiciumEngineFreeCustomContext(PINDICIUM_ENGINE Engine)
{
    if (!Engine) {
        return INDICIUM_ERROR_INVALID_ENGINE_HANDLE;
    }

    if (Engine->CustomContext) {
        free(Engine->CustomContext);
    }

    return INDICIUM_ERROR_NONE;
}

INDICIUM_API PVOID IndiciumEngineGetCustomContext(PINDICIUM_ENGINE Engine)
{
    if (!Engine) {
        return nullptr;
    }

    return Engine->CustomContext;
}

INDICIUM_API PINDICIUM_ENGINE IndiciumEngineGetHandleFromDXGISwapChain(IDXGISwapChain *pSwapChain)
{
    for (const auto& kv : g_EngineHostInstances)
    {
        const auto& engine = kv.second;

        if (engine->RenderPipeline.pSwapChain == pSwapChain) {
            return engine;
        }
    }

    return nullptr;
}

INDICIUM_API PINDICIUM_ENGINE IndiciumEngineGetHandleFromARC(IAudioRenderClient * client)
{
    for (const auto& kv : g_EngineHostInstances)
    {
        const auto& engine = kv.second;

        if (engine->CoreAudio.pARC == client) {
            return engine;
        }
    }

    return nullptr;
}

#ifndef INDICIUM_NO_D3D9

INDICIUM_API VOID IndiciumEngineSetD3D9EventCallbacks(PINDICIUM_ENGINE Engine, PINDICIUM_D3D9_EVENT_CALLBACKS Callbacks)
{
    if (Engine) {
        Engine->EventsD3D9 = *Callbacks;
    }
}

INDICIUM_API PINDICIUM_ENGINE IndiciumEngineGetHandleFromD3D9Device(LPDIRECT3DDEVICE9 Device)
{
    for (const auto& kv : g_EngineHostInstances)
    {
        const auto& engine = kv.second;

        if (engine->RenderPipeline.pD3D9Device == Device) {
            return engine;
        }
    }

    return nullptr;
}

INDICIUM_API PINDICIUM_ENGINE IndiciumEngineGetHandleFromD3D9ExDevice(LPDIRECT3DDEVICE9EX Device)
{
    for (const auto& kv : g_EngineHostInstances)
    {
        const auto& engine = kv.second;

        if (engine->RenderPipeline.pD3D9ExDevice == Device) {
            return engine;
        }
    }

    return nullptr;
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

#ifndef INDICIUM_NO_COREAUDIO

INDICIUM_API VOID IndiciumEngineSetARCEventCallbacks(PINDICIUM_ENGINE Engine, PINDICIUM_ARC_EVENT_CALLBACKS Callbacks)
{
    if (Engine) {
        Engine->EventsARC = *Callbacks;
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
