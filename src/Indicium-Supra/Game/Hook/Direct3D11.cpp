#include "Direct3D11.h"
#include "DXGI.h"
#include "Utils/Misc.h"


Direct3D11Hooking::Direct3D11::Direct3D11() : Direct3DBase(), vtableSwapChain(nullptr), pd3dDevice(nullptr), pd3dDeviceContext(nullptr), pSwapChain(nullptr)
{
    auto& logger = Logger::get(LOG_REGION());

	logger.information("Acquiring VTable for ID3D11Device and IDXGISwapChain...");

	auto hModDXGI = GetModuleHandle("DXGI.dll");
	auto hModD3D11 = GetModuleHandle("D3D11.dll");

	if (hModDXGI == nullptr)
	{
        logger.error("Couldn't get handle to DXGI.dll");
		return;
	}

	if (hModD3D11 == nullptr)
	{
        logger.error("Couldn't get handle to D3D11.dll");
		return;
	}

	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	{
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = temp_window.GetWindowHandle();
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	UINT createDeviceFlags = 0;
	D3D_FEATURE_LEVEL featureLevel;
    // Note: requesting lower feature level in case of missing hardware support
	const D3D_FEATURE_LEVEL featureLevelArray[3] = { D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

	auto hD3D11CreateDeviceAndSwapChain = static_cast<LPVOID>(GetProcAddress(hModD3D11, "D3D11CreateDeviceAndSwapChain"));
	if (hD3D11CreateDeviceAndSwapChain == nullptr)
	{
        logger.error("Couldn't get handle to D3D11CreateDeviceAndSwapChain");
		return;
	}

	auto hr11 = static_cast<HRESULT(WINAPI *)(
		IDXGIAdapter*,
		D3D_DRIVER_TYPE,
		HMODULE,
		UINT,
		const D3D_FEATURE_LEVEL*,
		UINT,
		UINT,
		const DXGI_SWAP_CHAIN_DESC*,
		IDXGISwapChain**,
		ID3D11Device**,
		D3D_FEATURE_LEVEL*,
		ID3D11DeviceContext**)>(hD3D11CreateDeviceAndSwapChain)(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createDeviceFlags,
		featureLevelArray,
		1,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pd3dDevice,
		&featureLevel,
		&pd3dDeviceContext);

	if (FAILED(hr11))
	{
        logger.error("Couldn't create D3D11 device");
		return;
	}

	vtable = *reinterpret_cast<UINTX **>(pd3dDevice);
	vtableSwapChain = *reinterpret_cast<UINTX **>(pSwapChain);

	logger.information("VTable acquired");
}


Direct3D11Hooking::Direct3D11::~Direct3D11()
{
    auto& logger = Logger::get(LOG_REGION());

	logger.information("Releasing temporary objects");

	if (pSwapChain)
		pSwapChain->Release();

	if (pd3dDevice)
		pd3dDevice->Release();

	if (pd3dDeviceContext)
		pd3dDeviceContext->Release();
}

bool Direct3D11Hooking::Direct3D11::GetDeviceVTable(UINTX* pVTable) const
{
	if (vtable)
	{
		memcpy(pVTable, vtable, VTableElements * sizeof(UINTX));
		return true;
	}

	return false;
}

bool Direct3D11Hooking::Direct3D11::GetSwapChainVTable(UINTX* pVTable) const
{
	if (vtableSwapChain)
	{
		memcpy(pVTable, vtableSwapChain, DXGIHooking::DXGI::SwapChainVTableElements * sizeof(UINTX));
		return true;
	}

	return false;
}
