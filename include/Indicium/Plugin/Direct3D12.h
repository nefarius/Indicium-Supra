#ifndef Direct3D12_h__
#define Direct3D12_h__

#include "Common.h"
#include <dxgi.h>

INDICIUM_EXPORT VOID indicium_plugin_d3d12_present(
    IDXGISwapChain  *pSwapChain,
    UINT            SyncInterval,
    UINT            Flags
);

INDICIUM_EXPORT VOID indicium_plugin_d3d12_resizetarget(
    IDXGISwapChain          *pSwapChain,
    const DXGI_MODE_DESC    *pNewTargetParameters
);

#endif // Direct3D12_h__
