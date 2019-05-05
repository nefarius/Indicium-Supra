#ifndef IndiciumDirect3D9_h__
#define IndiciumDirect3D9_h__

#include <d3d9.h>

typedef
_Function_class_(EVT_INDICIUM_D3D9_PRESENT)
VOID
EVT_INDICIUM_D3D9_PRESENT(
    LPDIRECT3DDEVICE9   pDevice,
    const RECT          *pSourceRect,
    const RECT          *pDestRect,
    HWND                hDestWindowOverride,
    const RGNDATA       *pDirtyRegion
);

typedef EVT_INDICIUM_D3D9_PRESENT *PFN_INDICIUM_D3D9_PRESENT;

typedef
_Function_class_(EVT_INDICIUM_D3D9_RESET)
VOID
EVT_INDICIUM_D3D9_RESET(
    LPDIRECT3DDEVICE9       pDevice,
    D3DPRESENT_PARAMETERS   *pPresentationParameters
);

typedef EVT_INDICIUM_D3D9_RESET *PFN_INDICIUM_D3D9_RESET;

typedef
_Function_class_(EVT_INDICIUM_D3D9_END_SCENE)
VOID
EVT_INDICIUM_D3D9_END_SCENE(
    LPDIRECT3DDEVICE9 pDevice
);

typedef EVT_INDICIUM_D3D9_END_SCENE *PFN_INDICIUM_D3D9_END_SCENE;

typedef
_Function_class_(EVT_INDICIUM_D3D9_PRESENT_EX)
VOID
EVT_INDICIUM_D3D9_PRESENT_EX(
    LPDIRECT3DDEVICE9EX     pDevice,
    const RECT              *pSourceRect,
    const RECT              *pDestRect,
    HWND                    hDestWindowOverride,
    const RGNDATA           *pDirtyRegion,
    DWORD                   dwFlags
);

typedef EVT_INDICIUM_D3D9_PRESENT_EX *PFN_INDICIUM_D3D9_PRESENT_EX;

typedef
_Function_class_(EVT_INDICIUM_D3D9_RESET_EX)
VOID
EVT_INDICIUM_D3D9_RESET_EX(
    LPDIRECT3DDEVICE9EX     pDevice,
    D3DPRESENT_PARAMETERS   *pPresentationParameters,
    D3DDISPLAYMODEEX        *pFullscreenDisplayMode
);

typedef EVT_INDICIUM_D3D9_RESET_EX *PFN_INDICIUM_D3D9_RESET_EX;


typedef struct _INDICIUM_D3D9_EVENT_CALLBACKS
{
    PFN_INDICIUM_D3D9_PRESENT       EvtIndiciumD3D9PrePresent;
    PFN_INDICIUM_D3D9_PRESENT       EvtIndiciumD3D9PostPresent;

    PFN_INDICIUM_D3D9_RESET         EvtIndiciumD3D9PreReset;
    PFN_INDICIUM_D3D9_RESET         EvtIndiciumD3D9PostReset;

    PFN_INDICIUM_D3D9_END_SCENE     EvtIndiciumD3D9PreEndScene;
    PFN_INDICIUM_D3D9_END_SCENE     EvtIndiciumD3D9PostEndScene;

    PFN_INDICIUM_D3D9_PRESENT_EX    EvtIndiciumD3D9PrePresentEx;
    PFN_INDICIUM_D3D9_PRESENT_EX    EvtIndiciumD3D9PostPresentEx;

    PFN_INDICIUM_D3D9_RESET_EX      EvtIndiciumD3D9PreResetEx;
    PFN_INDICIUM_D3D9_RESET_EX      EvtIndiciumD3D9PostResetEx;

} INDICIUM_D3D9_EVENT_CALLBACKS, *PINDICIUM_D3D9_EVENT_CALLBACKS;

/**
 * \fn  VOID FORCEINLINE INDICIUM_D3D9_EVENT_CALLBACKS_INIT( _Out_ PINDICIUM_D3D9_EVENT_CALLBACKS Callbacks )
 *
 * \brief   The Direct3D 9(Ex) event callback collection to initialize.
 *
 * \author  Benjamin Höglinger-Stelzer
 * \date    06.05.2019
 *
 * \param   Callbacks   The callback collection.
 *
 * \returns Nothing.
 */
VOID FORCEINLINE INDICIUM_D3D9_EVENT_CALLBACKS_INIT(
    _Out_ PINDICIUM_D3D9_EVENT_CALLBACKS Callbacks
)
{
    ZeroMemory(Callbacks, sizeof(INDICIUM_D3D9_EVENT_CALLBACKS));
}

#endif // IndiciumDirect3D9_h__
