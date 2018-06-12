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

#define INVOKE_INDICIUM_GAME_HOOKED(_engine_, _version_)    \
                                    (_engine_->EvtIndiciumGameHooked ? \
                                    _engine_->EvtIndiciumGameHooked(_version_) : \
                                    (void)0)

#define INVOKE_D3D9_CALLBACK(_engine_, _callback_, ...)     \
                            (_engine_->EventsD3D9._callback_ ? \
                            _engine_->EventsD3D9._callback_(##__VA_ARGS__) : \
                            (void)0)

#define INVOKE_D3D10_CALLBACK(_engine_, _callback_, ...)     \
                             (_engine_->EventsD3D10._callback_ ? \
                             _engine_->EventsD3D10._callback_(##__VA_ARGS__) : \
                             (void)0)

#define INVOKE_D3D11_CALLBACK(_engine_, _callback_, ...)     \
                             (_engine_->EventsD3D11._callback_ ? \
                             _engine_->EventsD3D11._callback_(##__VA_ARGS__) : \
                             (void)0)

#define INVOKE_D3D12_CALLBACK(_engine_, _callback_, ...)     \
                             (_engine_->EventsD3D12._callback_ ? \
                             _engine_->EventsD3D12._callback_(##__VA_ARGS__) : \
                             (void)0)
