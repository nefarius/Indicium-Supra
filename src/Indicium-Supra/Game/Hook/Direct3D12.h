#pragma once
#include "Direct3DBase.h"
#include <dxgi.h>
#include <d3d12.h>

namespace Direct3D12Hooking
{
    class Direct3D12 :
        public Direct3DHooking::Direct3DBase
    {
        ID3D12Device* pd3dDevice;
        ID3D12CommandQueue* pQueue;
        IDXGISwapChain* pSwapChain;

    public:
        Direct3D12();

        std::vector<UINTX> vtable() const override;
    protected:
        ~Direct3D12();
    };
}
