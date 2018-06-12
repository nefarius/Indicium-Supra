#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Indicium/Engine/IndiciumCore.h"
#include "Indicium/Engine/IndiciumDirect3D9.h"
#include "Indicium/Engine/IndiciumDirect3D10.h"
#include "Indicium/Engine/IndiciumDirect3D11.h"
#include "Indicium/Engine/IndiciumDirect3D12.h"


typedef struct _INDICIUM_ENGINE
{
    struct {
        PFN_INDICIUM_D3D9_PRESENT PrePresent;
        PFN_INDICIUM_D3D9_PRESENT PostPresent;

        PFN_INDICIUM_D3D9_RESET PreReset;
        PFN_INDICIUM_D3D9_RESET PostReset;

        PFN_INDICIUM_D3D9_END_SCENE PreEndScene;
        PFN_INDICIUM_D3D9_END_SCENE PostEndScene;

        PFN_INDICIUM_D3D9_PRESENT_EX PrePresentEx;
        PFN_INDICIUM_D3D9_PRESENT_EX PostPresentEx;

        PFN_INDICIUM_D3D9_RESET_EX PreResetEx;
        PFN_INDICIUM_D3D9_RESET_EX PostResetEx;
    } EventsD3D9;

    struct {
        PFN_INDICIUM_D3D10_PRESENT PrePresent;
        PFN_INDICIUM_D3D10_PRESENT PostPresent;

        PFN_INDICIUM_D3D10_RESIZE_TARGET PreResizeTarget;
        PFN_INDICIUM_D3D10_RESIZE_TARGET PostResizeTarget;
    } EventsD3D10;

    struct {
        PFN_INDICIUM_D3D11_PRESENT PrePresent;
        PFN_INDICIUM_D3D11_PRESENT PostPresent;

        PFN_INDICIUM_D3D11_RESIZE_TARGET PreResizeTarget;
        PFN_INDICIUM_D3D11_RESIZE_TARGET PostResizeTarget;
    } EventsD3D11;

    struct {
        PFN_INDICIUM_D3D12_PRESENT PrePresent;
        PFN_INDICIUM_D3D12_PRESENT PostPresent;

        PFN_INDICIUM_D3D12_RESIZE_TARGET PreResizeTarget;
        PFN_INDICIUM_D3D12_RESIZE_TARGET PostResizeTarget;
    } EventsD3D12;
} INDICIUM_ENGINE;
