#include "Direct3D10.h"
#include <vector>
#include "DXGI.h"
#include "Utils/Misc.h"


Direct3D10Hooking::Direct3D10::Direct3D10() : Direct3DBase(), vtableSwapChain(nullptr), pDevice(nullptr), pSwapChain(nullptr)
{
    auto& logger = Logger::get(LOG_REGION());

	logger.information("Acquiring VTable for ID3D10Device and IDXGISwapChain...");

	auto hModDXGI = GetModuleHandle("DXGI.dll");
	auto hModD3D10 = GetModuleHandle("D3D10.dll");

	if (hModDXGI == nullptr)
	{
        logger.error("Couldn't get handle to DXGI.dll");
		return;
	}

	if (hModD3D10 == nullptr)
	{
        logger.error("Couldn't get handle to D3D10.dll");
		return;
	}

	auto hCreateDXGIFactory = static_cast<LPVOID>(GetProcAddress(hModDXGI, "CreateDXGIFactory"));
	if (hCreateDXGIFactory == nullptr)
	{
        logger.error("Couldn't get handle to CreateDXGIFactory");
		return;
	}

	IDXGIFactory* pFactory;
	auto hr = static_cast<HRESULT(WINAPI *)(
		REFIID, 
		void** ppFactory)>(hCreateDXGIFactory)(
		__uuidof(IDXGIFactory), 
		reinterpret_cast<void**>(&pFactory));

	if (FAILED(hr))
	{
        logger.error("Couldn't create DXGI factory");
		return;
	}

	auto hD3D10CreateDeviceAndSwapChain = static_cast<LPVOID>(GetProcAddress(hModD3D10, "D3D10CreateDeviceAndSwapChain"));
	if (hD3D10CreateDeviceAndSwapChain == nullptr)
	{
        logger.error("Couldn't get handle to D3D10CreateDeviceAndSwapChain");
		return;
	}

	UINT i = 0;
	IDXGIAdapter* pAdapter;
	std::vector<IDXGIAdapter*> vAdapters;
	while (pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		vAdapters.push_back(pAdapter);
		++i;
	}

	if (vAdapters.empty())
	{
        logger.error("No adapters found");
		return;
	}

	DXGI_RATIONAL rational = {};
	rational.Numerator = 60;
	rational.Denominator = 1;

	DXGI_MODE_DESC modeDesc = {};
	modeDesc.Height = 100;
	modeDesc.Width = 100;
	modeDesc.RefreshRate = rational;
	modeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	DXGI_SAMPLE_DESC sampleDesc = {};
	sampleDesc.Count = 1;
	sampleDesc.Quality = 0;

	DXGI_SWAP_CHAIN_DESC scDesc = {};
	scDesc.BufferCount = 1;
	scDesc.Flags = 0;
	scDesc.Windowed = true;
	scDesc.OutputWindow = temp_window.GetWindowHandle();
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.SampleDesc = sampleDesc;
	scDesc.BufferDesc = modeDesc;

	auto hr10 = static_cast<HRESULT(WINAPI *)(
		IDXGIAdapter*, 
		D3D10_DRIVER_TYPE, 
		HMODULE, 
		UINT, 
		UINT, 
		DXGI_SWAP_CHAIN_DESC*, 
		IDXGISwapChain**, 
		ID3D10Device**)>(hD3D10CreateDeviceAndSwapChain)(
		vAdapters[0], 
		D3D10_DRIVER_TYPE_HARDWARE, 
		NULL, 
		0, 
		D3D10_SDK_VERSION, 
		&scDesc, 
		&pSwapChain, 
		&pDevice);

	if (FAILED(hr10))
	{
        logger.error("Couldn't create D3D10 device");
		return;
	}

	vtable = *reinterpret_cast<UINTX **>(pDevice);
	vtableSwapChain = *reinterpret_cast<UINTX **>(pSwapChain);

	logger.information("VTable acquired");
}


Direct3D10Hooking::Direct3D10::~Direct3D10()
{
    auto& logger = Logger::get(LOG_REGION());

	logger.information("Releasing temporary objects");

	if (pSwapChain)
		pSwapChain->Release();

	if (pDevice)
		pDevice->Release();
}

bool Direct3D10Hooking::Direct3D10::GetDeviceVTable(UINTX* pVTable) const
{
	if (vtable)
	{
		memcpy(pVTable, vtable, VTableElements * sizeof(UINTX));
		return true;
	}

	return false;
}

bool Direct3D10Hooking::Direct3D10::GetSwapChainVTable(UINTX* pVTable) const
{
	if (vtableSwapChain)
	{
		memcpy(pVTable, vtableSwapChain, DXGIHooking::DXGI::SwapChainVTableElements * sizeof(UINTX));
		return true;
	}

	return false;
}
