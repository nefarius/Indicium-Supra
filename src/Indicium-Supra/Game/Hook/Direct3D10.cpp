/*
MIT License

Copyright (c) 2018 Benjamin Höglinger

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Direct3D10.h"
#include <Poco/Exception.h>
#include <vector>
#include "DXGI.h"
#include "Utils/Misc.h"


Direct3D10Hooking::Direct3D10::Direct3D10() : Direct3DBase(), pDevice(nullptr), pSwapChain(nullptr)
{
    temp_window = new Window("TempDirect3D10OverlayWindow");

	auto hModDXGI = GetModuleHandle("DXGI.dll");
	auto hModD3D10 = GetModuleHandle("D3D10.dll");

	if (hModDXGI == nullptr)
	{
        throw Poco::RuntimeException("Couldn't get handle to DXGI.dll", GetLastError());
	}

	if (hModD3D10 == nullptr)
	{
        throw Poco::RuntimeException("Couldn't get handle to D3D10.dll", GetLastError());
	}

	auto hCreateDXGIFactory = static_cast<LPVOID>(GetProcAddress(hModDXGI, "CreateDXGIFactory"));
	if (hCreateDXGIFactory == nullptr)
	{
        throw Poco::RuntimeException("Couldn't get handle to CreateDXGIFactory", GetLastError());
	}

	IDXGIFactory* pFactory;
	auto hr = static_cast<HRESULT(WINAPI *)(
		REFIID, 
		void** ppFactory)>(hCreateDXGIFactory)(
		__uuidof(IDXGIFactory), 
		reinterpret_cast<void**>(&pFactory));

	if (FAILED(hr))
	{
        throw Poco::RuntimeException("Couldn't create DXGI factory", hr);
	}

	auto hD3D10CreateDeviceAndSwapChain = static_cast<LPVOID>(GetProcAddress(hModD3D10, "D3D10CreateDeviceAndSwapChain"));
	if (hD3D10CreateDeviceAndSwapChain == nullptr)
	{
        throw Poco::RuntimeException("Couldn't get handle to D3D10CreateDeviceAndSwapChain", GetLastError());
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
        throw Poco::RuntimeException("No adapters found");
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
	scDesc.OutputWindow = temp_window->windowHandle();
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
        throw Poco::RuntimeException("Couldn't create D3D10 device");
	}
}

std::vector<UINTX> Direct3D10Hooking::Direct3D10::vtable() const
{
    UINTX vtbl[DXGIHooking::DXGI::SwapChainVTableElements];
    memcpy(vtbl, *reinterpret_cast<UINTX **>(pSwapChain), DXGIHooking::DXGI::SwapChainVTableElements * sizeof(UINTX));
    return std::vector<UINTX>(vtbl, vtbl + sizeof vtbl / sizeof vtbl[0]);
}

Direct3D10Hooking::Direct3D10::~Direct3D10()
{
	if (pSwapChain)
		pSwapChain->Release();

	if (pDevice)
		pDevice->Release();
}
