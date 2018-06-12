#ifndef IndiciumDirect3D12_h__
#define IndiciumDirect3D12_h__

#include <dxgi.h>

typedef
_Function_class_(EVT_INDICIUM_D3D12_PRESENT)
VOID
EVT_INDICIUM_D3D12_PRESENT(
    IDXGISwapChain  *pSwapChain,
    UINT            SyncInterval,
    UINT            Flags
);

typedef EVT_INDICIUM_D3D12_PRESENT *PFN_INDICIUM_D3D12_PRESENT;

typedef
_Function_class_(EVT_INDICIUM_D3D12_RESIZE_TARGET)
VOID
EVT_INDICIUM_D3D12_RESIZE_TARGET(
    IDXGISwapChain          *pSwapChain,
    const DXGI_MODE_DESC    *pNewTargetParameters
);

typedef EVT_INDICIUM_D3D12_RESIZE_TARGET *PFN_INDICIUM_D3D12_RESIZE_TARGET;

#endif // IndiciumDirect3D12_h__
