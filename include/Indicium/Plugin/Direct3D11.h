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

#ifndef Direct3D11_h__
#define Direct3D11_h__

#include "Common.h"
#include <dxgi.h>
#include <d3d11.h>

typedef
_Function_class_(EVT_INDICIUM_D3D11_PRESENT)
VOID
EVT_INDICIUM_D3D11_PRESENT(
    IDXGISwapChain  *pSwapChain,
    UINT            SyncInterval,
    UINT            Flags
);

typedef EVT_INDICIUM_D3D11_PRESENT *PFN_INDICIUM_D3D11_PRESENT;

typedef
_Function_class_(EVT_INDICIUM_D3D11_RESIZE_TARGET)
VOID
EVT_INDICIUM_D3D11_RESIZE_TARGET(
    IDXGISwapChain          *pSwapChain,
    const DXGI_MODE_DESC    *pNewTargetParameters
);

typedef EVT_INDICIUM_D3D11_RESIZE_TARGET *PFN_INDICIUM_D3D11_RESIZE_TARGET;

#pragma region DEPRECATED

INDICIUM_EXPORT(VOID) indicium_plugin_d3d11_present(
    IDXGISwapChain  *pSwapChain,
    UINT            SyncInterval,
    UINT            Flags
);

INDICIUM_EXPORT(VOID) indicium_plugin_d3d11_resizetarget(
    IDXGISwapChain          *pSwapChain,
    const DXGI_MODE_DESC    *pNewTargetParameters
);

#pragma endregion

HRESULT 
FORCEINLINE 
D3D11_DEVICE_CONTEXT_FROM_SWAPCHAIN(
    IDXGISwapChain          *pSwapChain,
    ID3D11Device            **ppDevice,
    ID3D11DeviceContext     **ppContext
)
{
    HRESULT ret = pSwapChain->GetDevice(__uuidof(ID3D11Device), (PVOID*)ppDevice);

    if (SUCCEEDED(ret))
        (*ppDevice)->GetImmediateContext(ppContext);

    return ret;
}

#endif // Direct3D11_h__
