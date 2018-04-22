#ifndef Direct3D11_h__
#define Direct3D11_h__

#include "Common.h"
#include <dxgi.h>
#include <d3d11.h>

INDICIUM_EXPORT VOID indicium_plugin_d3d11_present(
    IDXGISwapChain  *pSwapChain,
    UINT            SyncInterval,
    UINT            Flags
);

INDICIUM_EXPORT VOID indicium_plugin_d3d11_resizetarget(
    IDXGISwapChain          *pSwapChain,
    const DXGI_MODE_DESC    *pNewTargetParameters
);

HRESULT FORCEINLINE D3D11_DEVICE_CONTEXT_FROM_SWAPCHAIN(
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
