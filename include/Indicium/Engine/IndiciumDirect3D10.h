#ifndef IndiciumDirect3D10_h__
#define IndiciumDirect3D10_h__

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

HRESULT
FORCEINLINE
D3D10_DEVICE_FROM_SWAPCHAIN(
    IDXGISwapChain *pSwapChain,
    ID3D10Device **ppDevice
)
{
    return pSwapChain->GetDevice(__uuidof(ID3D10Device), (PVOID*)ppDevice);
}

#endif // IndiciumDirect3D10_h__
