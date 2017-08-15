#pragma once
#include "Direct3DBase.h"
#include <dxgi.h>
#include <d3d12.h>

namespace Direct3D12Hooking
{
    class Direct3D12 :
        public Direct3DHooking::Direct3DBase
    {
        UINTX					*vtableSwapChain;
        ID3D12Device            *pd3dDevice;
        ID3D12CommandQueue      *pQueue;
        IDXGISwapChain          *pSwapChain;

    public:
        Direct3D12();
        ~Direct3D12();

        bool GetDeviceVTable(UINTX* pVTable) const override;
        bool GetSwapChainVTable(UINTX *pVTable) const;
    };
}
