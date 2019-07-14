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


#ifndef IndiciumDirect3D10_h__
#define IndiciumDirect3D10_h__

#ifndef INDICIUM_NO_D3D10

#include <dxgi.h>
#include <d3d10_1.h>

typedef
_Function_class_(EVT_INDICIUM_D3D10_PRESENT)
VOID
EVT_INDICIUM_D3D10_PRESENT(
    IDXGISwapChain  *pSwapChain,
    UINT            SyncInterval,
    UINT            Flags
);

typedef EVT_INDICIUM_D3D10_PRESENT *PFN_INDICIUM_D3D10_PRESENT;

typedef
_Function_class_(EVT_INDICIUM_D3D10_RESIZE_TARGET)
VOID
EVT_INDICIUM_D3D10_RESIZE_TARGET(
    IDXGISwapChain          *pSwapChain,
    const DXGI_MODE_DESC    *pNewTargetParameters
);

typedef EVT_INDICIUM_D3D10_RESIZE_TARGET *PFN_INDICIUM_D3D10_RESIZE_TARGET;

typedef
_Function_class_(EVT_INDICIUM_D3D10_RESIZE_BUFFERS)
VOID
EVT_INDICIUM_D3D10_RESIZE_BUFFERS(
    IDXGISwapChain  *pSwapChain,
    UINT            BufferCount,
    UINT            Width,
    UINT            Height,
    DXGI_FORMAT     NewFormat,
    UINT            SwapChainFlags
);

typedef EVT_INDICIUM_D3D10_RESIZE_BUFFERS *PFN_INDICIUM_D3D10_RESIZE_BUFFERS;

HRESULT
FORCEINLINE
D3D10_DEVICE_FROM_SWAPCHAIN(
    IDXGISwapChain *pSwapChain,
    ID3D10Device **ppDevice
)
{
    return pSwapChain->GetDevice(__uuidof(ID3D10Device), (PVOID*)ppDevice);
}


typedef struct _INDICIUM_D3D10_EVENT_CALLBACKS
{
    PFN_INDICIUM_D3D10_PRESENT          EvtIndiciumD3D10PrePresent;
    PFN_INDICIUM_D3D10_PRESENT          EvtIndiciumD3D10PostPresent;

    PFN_INDICIUM_D3D10_RESIZE_TARGET    EvtIndiciumD3D10PreResizeTarget;
    PFN_INDICIUM_D3D10_RESIZE_TARGET    EvtIndiciumD3D10PostResizeTarget;

    PFN_INDICIUM_D3D10_RESIZE_BUFFERS   EvtIndiciumD3D10PreResizeBuffers;
    PFN_INDICIUM_D3D10_RESIZE_BUFFERS   EvtIndiciumD3D10PostResizeBuffers;

} INDICIUM_D3D10_EVENT_CALLBACKS, *PINDICIUM_D3D10_EVENT_CALLBACKS;

/**
 * \fn  VOID FORCEINLINE INDICIUM_D3D10_EVENT_CALLBACKS_INIT( _Out_ PINDICIUM_D3D10_EVENT_CALLBACKS Callbacks )
 *
 * \brief   The Direct3D 10 event callback collection to initialize.
 *
 * \author  Benjamin Höglinger-Stelzer
 * \date    06.05.2019
 *
 * \param   Callbacks   The callback collection.
 *
 * \returns Nothing.
 */
VOID FORCEINLINE INDICIUM_D3D10_EVENT_CALLBACKS_INIT(
    _Out_ PINDICIUM_D3D10_EVENT_CALLBACKS Callbacks
)
{
    ZeroMemory(Callbacks, sizeof(INDICIUM_D3D10_EVENT_CALLBACKS));
}

#endif

#endif // IndiciumDirect3D10_h__
