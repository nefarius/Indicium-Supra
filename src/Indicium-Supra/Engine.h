#pragma once

#include <Poco/AutoPtr.h>
#include <Poco/Util/IniFileConfiguration.h>

//
// Internal engine instance properties
//
typedef struct _INDICIUM_ENGINE
{
    //
    // Detected Direct3D version the host process is using to render
    //
    INDICIUM_D3D_VERSION GameVersion;

    //
    // Callback to be invoked once the render pipeline has been hooked
    //
    PFN_INDICIUM_GAME_HOOKED EvtIndiciumGameHooked;

    //
    // Direct3D 9(Ex) specific render pipeline callbacks
    //
    INDICIUM_D3D9_EVENT_CALLBACKS EventsD3D9;

    //
    // Direct3D 10 specific render pipeline callbacks
    //
    INDICIUM_D3D10_EVENT_CALLBACKS EventsD3D10;

    //
    // Direct3D 11 specific render pipeline callbacks
    //
    INDICIUM_D3D11_EVENT_CALLBACKS EventsD3D11;

    //
    // Direct3D 12 specific render pipeline callbacks
    //
    INDICIUM_D3D12_EVENT_CALLBACKS EventsD3D12;

    //
    // Handle to main worker thread holding the hooks
    //
    HANDLE EngineThread;

    //
    // Signals the main thread to terminate
    //
    HANDLE EngineCancellationEvent;

    //
    // Signals the caller of the API that shutdown has occurred
    //
    HANDLE EngineCancellationCompletedEvent;

    //
    // Runtime configuration
    //
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
