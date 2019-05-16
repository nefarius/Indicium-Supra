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


#pragma once

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
