#pragma once
#include "Window.h"
#include <dxgi.h>
#include <d3d10.h>
#include <intarch.h>
#include "Direct3DBase.h"

namespace Direct3D10Hooking
{
	enum DXGISwapChainVTbl : short
	{
		// IUnknown
		QueryInterface = 0,
		AddRef = 1,
		Release = 2,

		// IDXGIObject
		SetPrivateData = 3,
		SetPrivateDataInterface = 4,
		GetPrivateData = 5,
		GetParent = 6,

		// IDXGIDeviceSubObject
		GetDevice = 7,

		// IDXGISwapChain
		Present = 8,
		GetBuffer = 9,
		SetFullscreenState = 10,
		GetFullscreenState = 11,
		GetDesc = 12,
		ResizeBuffers = 13,
		ResizeTarget = 14,
		GetContainingOutput = 15,
		GetFrameStatistics = 16,
		GetLastPresentCount = 17
	};
	
	class Direct3D10 : Direct3DHooking::Direct3DBase
	{
		UINTX *vtableSwapChain;
		ID3D10Device *pDevice;
		IDXGISwapChain *pSwapChain;

	public:
		Direct3D10();
		~Direct3D10();
		static const int VTableElements = 98;
		static const int SwapChainVTableElements = 18;

		bool GetDeviceVTable(UINTX* pVTable) const override;
		bool GetSwapChainVTable(UINTX *pVTable) const;
	};
}
