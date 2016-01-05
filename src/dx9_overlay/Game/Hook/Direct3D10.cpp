#include "Direct3D10.h"
#include <boost/log/trivial.hpp>
#include <vector>


Direct3D10Hooking::Direct3D10::Direct3D10(): vtable(nullptr), vtableSwapChain(nullptr), pDevice(nullptr), pSwapChain(nullptr)
{
	temp_window = new Window();

	BOOST_LOG_TRIVIAL(info) << "Acquiring VTable for ID3D10Device and IDXGISwapChain...";

	IDXGIFactory * pFactory;
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));

	if (FAILED(hr))
	{
		BOOST_LOG_TRIVIAL(error) << "Couldn't create DXGI factory";
		return;
	}

	UINT i = 0;
	IDXGIAdapter * pAdapter;
	std::vector<IDXGIAdapter*> vAdapters;
	while (pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		vAdapters.push_back(pAdapter);
		++i;
	}

	if (vAdapters.empty())
	{
		BOOST_LOG_TRIVIAL(error) << "No adapters found";
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
	scDesc.OutputWindow = temp_window->GetWindowHandle();
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.SampleDesc = sampleDesc;
	scDesc.BufferDesc = modeDesc;

	if (FAILED(D3D10CreateDeviceAndSwapChain(vAdapters[0], D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION, &scDesc, &pSwapChain, &pDevice)))
	{
		BOOST_LOG_TRIVIAL(error) << "Couldn't create D3D10 device";
		return;
	}

#ifdef _M_IX86
	vtable = *reinterpret_cast<UINT32 **>(pDevice);
	vtableSwapChain = *reinterpret_cast<UINT32 **>(pSwapChain);
#else
	vtable = *reinterpret_cast<UINT64 **>(pDevice);
	vtableSwapChain = *reinterpret_cast<UINT64 **>(pSwapChain);
#endif

	BOOST_LOG_TRIVIAL(info) << "VTable acquired";
}


Direct3D10Hooking::Direct3D10::~Direct3D10()
{
	BOOST_LOG_TRIVIAL(info) << "Releasing temporary objects";

	if (pSwapChain)
		pSwapChain->Release();

	if (pDevice)
		pDevice->Release();

	if (temp_window)
		delete temp_window;
}

bool Direct3D10Hooking::Direct3D10::GetSwapChainVTable(UINT32* pVTable) const
{
	if (vtableSwapChain)
	{
		memcpy(pVTable, vtableSwapChain, SwapChainVTableElements * sizeof(UINT32));
		return true;
	}
	
	return false;
}
