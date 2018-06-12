#ifndef IndiciumDirect3D11_h__
#define IndiciumDirect3D11_h__

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


typedef struct _INDICIUM_D3D11_EVENT_CALLBACKS
{
    ULONG Size;

    PFN_INDICIUM_D3D11_PRESENT          EvtIndiciumD3D11PrePresent;
    PFN_INDICIUM_D3D11_PRESENT          EvtIndiciumD3D11PostPresent;

    PFN_INDICIUM_D3D11_RESIZE_TARGET    EvtIndiciumD3D11PreResizeTarget;
    PFN_INDICIUM_D3D11_RESIZE_TARGET    EvtIndiciumD3D11PostResizeTarget;

} INDICIUM_D3D11_EVENT_CALLBACKS, *PINDICIUM_D3D11_EVENT_CALLBACKS;

VOID
FORCEINLINE
INDICIUM_D3D11_EVENT_CALLBACKS_INIT(
    _Out_ PINDICIUM_D3D11_EVENT_CALLBACKS Callbacks
)
{
    RtlZeroMemory(Callbacks, sizeof(INDICIUM_D3D11_EVENT_CALLBACKS));

    Callbacks->Size = sizeof(INDICIUM_D3D11_EVENT_CALLBACKS);
}

#endif // IndiciumDirect3D11_h__
