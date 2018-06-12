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
    auto engine = static_cast<PINDICIUM_ENGINE>(malloc(sizeof(INDICIUM_ENGINE)));

    if (!engine) {
        return nullptr;
    }

    RtlZeroMemory(engine, sizeof(INDICIUM_ENGINE));

    return engine;
}

INDICIUM_API INDICIUM_ERROR IndiciumEngineInit(PINDICIUM_ENGINE Engine, PFN_EVT_INDICIUM_GAME_HOOKED EvtIndiciumGameHooked)
{
    if (!Engine) {
        return INDICIUM_ERROR_INVALID_ENGINE_HANDLE;
    }

    if (Engine->EventsD3D9.Size != sizeof(INDICIUM_D3D9_EVENT_CALLBACKS)) {
        return INDICIUM_ERROR_INVALID_D3D9_CALLBACKS_SIZE;
    }

    if (Engine->EventsD3D10.Size != sizeof(INDICIUM_D3D10_EVENT_CALLBACKS)) {
        return INDICIUM_ERROR_INVALID_D3D10_CALLBACKS_SIZE;
    }

    if (Engine->EventsD3D11.Size != sizeof(INDICIUM_D3D11_EVENT_CALLBACKS)) {
        return INDICIUM_ERROR_INVALID_D3D11_CALLBACKS_SIZE;
    }

    if (Engine->EventsD3D12.Size != sizeof(INDICIUM_D3D12_EVENT_CALLBACKS)) {
        return INDICIUM_ERROR_INVALID_D3D12_CALLBACKS_SIZE;
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
    // Fetch configuration
    // 
    Path cfgFile(GlobalState::instance().rootPath(), "Indicium-Supra.ini");
    Engine->Configuration = new IniFileConfiguration();

    //
    // Set up logging
    // 
    AutoPtr<FileChannel> pFileChannel(new FileChannel);
    pFileChannel->setProperty("path", Path::expand(Engine->Configuration->getString("global.logFile", "%TEMP%\\Indicium-Supra.log")));
    AutoPtr<PatternFormatter> pPF(new PatternFormatter);
    pPF->setProperty("pattern", "%Y-%m-%d %H:%M:%S.%i %s [%p]: %t");
    AutoPtr<FormattingChannel> pFC(new FormattingChannel(pPF, pFileChannel));

    Logger::root().setChannel(pFC);

    auto& logger = Logger::get(__func__);

    logger.information("Indicium engine initialized, attempting to launch main thread");

    Engine->EngineThread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(IndiciumMainThread), Engine, 0, nullptr);

    if (!Engine->EngineThread) {
        logger.fatal("Couldn't create main thread, library unusable");
        return INDICIUM_ERROR_CREATE_THREAD_FAILED;
    }

    logger.information("Main thread created successfully");

    return INDICIUM_ERROR_NONE;
}

INDICIUM_API VOID IndiciumEngineShutdown(PINDICIUM_ENGINE Engine)
{
    if (!Engine) {
        return;
    }

    SetEvent(Engine->EngineCancellationEvent);
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
