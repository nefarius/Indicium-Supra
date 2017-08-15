#pragma once
#include "Direct3DBase.h"

namespace Direct3D12Hooking
{
    class Direct3D12 :
        public Direct3DHooking::Direct3DBase
    {
        UINTX					*vtableSwapChain;

    public:
        Direct3D12();
        ~Direct3D12();

        bool GetSwapChainVTable(UINTX *pVTable) const;
    };
}
