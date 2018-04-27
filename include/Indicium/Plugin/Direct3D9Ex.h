#ifndef Direct3D9Ex_h__
#define Direct3D9Ex_h__

#include "Common.h"
#include <d3d9.h>

INDICIUM_EXPORT(VOID) indicium_plugin_d3d9_presentex(
    LPDIRECT3DDEVICE9EX     pDevice,
    const RECT              *pSourceRect,
    const RECT              *pDestRect,
    HWND                    hDestWindowOverride,
    const RGNDATA           *pDirtyRegion,
    DWORD                   dwFlags
);

INDICIUM_EXPORT(VOID) indicium_plugin_d3d9_resetex(
    LPDIRECT3DDEVICE9EX     pDevice,
    D3DPRESENT_PARAMETERS   *pPresentationParameters,
    D3DDISPLAYMODEEX        *pFullscreenDisplayMode
);

#endif // Direct3D9Ex_h__
