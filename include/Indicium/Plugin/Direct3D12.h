#ifndef Direct3D12_h__
#define Direct3D12_h__

#ifndef IS_INDICIUM_SUPRA

#include <Windows.h>
#include <dxgi.h>

#define INDICIUM_EXPORT extern "C" __declspec(dllexport) VOID __cdecl

INDICIUM_EXPORT indicium_plugin_d3d12_present(
    IDXGISwapChain  *pSwapChain,
    UINT            SyncInterval,
    UINT            Flags
);

INDICIUM_EXPORT indicium_plugin_d3d12_resizetarget(
    IDXGISwapChain          *pSwapChain,
    const DXGI_MODE_DESC    *pNewTargetParameters
);

#endif

#endif // Direct3D12_h__
