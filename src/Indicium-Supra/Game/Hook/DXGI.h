#pragma once

namespace DXGIHooking
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

	namespace DXGI1
	{
		enum DXGISwapChain1VTbl : short
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
			GetLastPresentCount = 17,

			// IDXGISwapChain1
			GetDesc1 = 18,
			GetFullscreenDesc = 19,
			GetHwnd = 20,
			GetCoreWindow = 21,
			Present1 = 22,
			IsTemporaryMonoSupported = 23,
			GetRestrictToOutput = 24,
			SetBackgroundColor = 25,
			GetBackgroundColor = 26,
			SetRotation = 27,
			GetRotation = 28
		};
	}

	class DXGI
	{
	public:
		static const int SwapChainVTableElements = 18;
		static const int SwapChain1VTableElements = 29;
	};
}
