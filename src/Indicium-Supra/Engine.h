#pragma once

#include <Poco/AutoPtr.h>
#include <Poco/Util/IniFileConfiguration.h>

typedef struct _INDICIUM_ENGINE
{
    INDICIUM_D3D_VERSION GameVersion;

    PFN_EVT_INDICIUM_GAME_HOOKED EvtIndiciumGameHooked;

    INDICIUM_D3D9_EVENT_CALLBACKS EventsD3D9;

    INDICIUM_D3D10_EVENT_CALLBACKS EventsD3D10;

    INDICIUM_D3D11_EVENT_CALLBACKS EventsD3D11;

    INDICIUM_D3D12_EVENT_CALLBACKS EventsD3D12;

    HANDLE EngineThread;

    HANDLE EngineCancellationEvent;

    Poco::AutoPtr<Poco::Util::IniFileConfiguration> Configuration;

} INDICIUM_ENGINE;
