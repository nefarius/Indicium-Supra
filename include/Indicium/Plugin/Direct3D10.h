#ifndef Direct3D10_h__
#define Direct3D10_h__

#include "Common.h"
#include <dxgi.h>
#include <d3d10.h>

INDICIUM_EXPORT VOID indicium_plugin_d3d10_present(
    IDXGISwapChain  *pSwapChain,
    UINT            SyncInterval,
    UINT            Flags
);

INDICIUM_EXPORT VOID indicium_plugin_d3d10_resizetarget(
    IDXGISwapChain          *pSwapChain,
    const DXGI_MODE_DESC    *pNewTargetParameters
);

HRESULT FORCEINLINE D3D10_DEVICE_FROM_SWAPCHAIN(IDXGISwapChain *pSwapChain, ID3D10Device **ppDevice)
{
    return pSwapChain->GetDevice(__uuidof(ID3D10Device), (PVOID*)ppDevice);
}

#endif // Direct3D10_h__
