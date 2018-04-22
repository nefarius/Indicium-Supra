#ifndef Direct3D9_h__
#define Direct3D9_h__

#ifndef IS_INDICIUM_SUPRA

#include <Windows.h>
#include <d3d9.h>

#define INDICIUM_EXPORT extern "C" __declspec(dllexport) VOID __cdecl

INDICIUM_EXPORT indicium_plugin_d3d9_present(
    LPDIRECT3DDEVICE9   pDevice,
    const RECT          *pSourceRect,
    const RECT          *pDestRect,
    HWND                hDestWindowOverride,
    const RGNDATA       *pDirtyRegion
);

INDICIUM_EXPORT indicium_plugin_d3d9_reset(
    LPDIRECT3DDEVICE9       pDevice, 
    D3DPRESENT_PARAMETERS   *pPresentationParameters
);

INDICIUM_EXPORT indicium_plugin_d3d9_endscene(
    LPDIRECT3DDEVICE9 pDevice
);

#endif

#endif // Direct3D9_h__
