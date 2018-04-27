#ifndef Direct3D9_h__
#define Direct3D9_h__

#include "Common.h"
#include <d3d9.h>

INDICIUM_EXPORT(VOID) indicium_plugin_d3d9_present(
    LPDIRECT3DDEVICE9   pDevice,
    const RECT          *pSourceRect,
    const RECT          *pDestRect,
    HWND                hDestWindowOverride,
    const RGNDATA       *pDirtyRegion
);

INDICIUM_EXPORT(VOID) indicium_plugin_d3d9_reset(
    LPDIRECT3DDEVICE9       pDevice, 
    D3DPRESENT_PARAMETERS   *pPresentationParameters
);

INDICIUM_EXPORT(VOID) indicium_plugin_d3d9_endscene(
    LPDIRECT3DDEVICE9 pDevice
);

#endif // Direct3D9_h__
