#pragma once
#include "Direct3DBase.h"
#include <dxgi.h>
#include <d3d11.h>

namespace Direct3D11Hooking
{
	enum D3D11DeviceVTbl : short
	{
		// IUnknown
		QueryInterface = 0,
		AddRef = 1,
		Release = 2,

		// ID3D11Device
		CreateBuffer = 3,
		CreateTexture1D = 4,
		CreateTexture2D = 5,
		CreateTexture3D = 6,
		CreateShaderResourceView = 7,
		CreateUnorderedAccessView = 8,
		CreateRenderTargetView = 9,
		CreateDepthStencilView = 10,
		CreateInputLayout = 11,
		CreateVertexShader = 12,
		CreateGeometryShader = 13,
		CreateGeometryShaderWithStreamOutput = 14,
		CreatePixelShader = 15,
		CreateHullShader = 16,
		CreateDomainShader = 17,
		CreateComputeShader = 18,
		CreateClassLinkage = 19,
		CreateBlendState = 20,
		CreateDepthStencilState = 21,
		CreateRasterizerState = 22,
		CreateSamplerState = 23,
		CreateQuery = 24,
		CreatePredicate = 25,
		CreateCounter = 26,
		CreateDeferredContext = 27,
		OpenSharedResource = 28,
		CheckFormatSupport = 29,
		CheckMultisampleQualityLevels = 30,
		CheckCounterInfo = 31,
		CheckCounter = 32,
		CheckFeatureSupport = 33,
		GetPrivateData = 34,
		SetPrivateData = 35,
		SetPrivateDataInterface = 36,
		GetFeatureLevel = 37,
		GetCreationFlags = 38,
		GetDeviceRemovedReason = 39,
		GetImmediateContext = 40,
		SetExceptionMode = 41,
		GetExceptionMode = 42
	};

	class Direct3D11 :
		public Direct3DHooking::Direct3DBase
	{
		UINTX					*vtableSwapChain;
		ID3D11Device*            pd3dDevice;
		ID3D11DeviceContext*     pd3dDeviceContext;
		IDXGISwapChain*          pSwapChain;
	public:
		Direct3D11();
		~Direct3D11();
		static const int VTableElements = 43;

		bool GetDeviceVTable(UINTX* pVTable) const override;
		bool GetSwapChainVTable(UINTX *pVTable) const;
	};
}
