#include "Direct3D9.h"

#include <Poco/Exception.h>


Direct3D9Hooking::Direct3D9::Direct3D9() : Direct3DBase(), d3d9(nullptr), d3d9_device(nullptr)
{
    temp_window = new Window("TempDirect3D9OverlayWindow");

	auto hMod = GetModuleHandle("d3d9.dll");

	if (hMod == nullptr)
	{
		throw Poco::RuntimeException("Could not get the handle to d3d9.dll");
	}

    const auto Direct3DCreate9 = static_cast<LPVOID>(GetProcAddress(hMod, "Direct3DCreate9"));
	if (Direct3DCreate9 == nullptr)
	{
		throw Poco::RuntimeException("Could not locate the Direct3DCreate9 procedure entry point");
	}

	d3d9 = static_cast<LPDIRECT3D9(WINAPI *)(UINT)>(Direct3DCreate9)(D3D_SDK_VERSION);
	if (d3d9 == nullptr)
	{
		throw Poco::RuntimeException("Could not create the DirectX 9 interface");
	}

	D3DDISPLAYMODE display_mode;
	if (FAILED(d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display_mode)))
	{
		throw Poco::RuntimeException("Could not determine the current display mode");
	}

	D3DPRESENT_PARAMETERS present_parameters;
	ZeroMemory(&present_parameters, sizeof(D3DPRESENT_PARAMETERS));
	present_parameters.Windowed = TRUE;
	present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	present_parameters.BackBufferFormat = display_mode.Format;

    const auto error_code = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, temp_window->windowHandle(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT, &present_parameters, &d3d9_device);
	if (FAILED(error_code))
	{
		throw Poco::RuntimeException("Could not create the Direct3D 9 device");
	}

	vtable = *reinterpret_cast<UINTX **>(d3d9_device);
}

Direct3D9Hooking::Direct3D9::~Direct3D9()
{
	if (d3d9_device)
		d3d9_device->Release();

	if (d3d9)
		d3d9->Release();
}

bool Direct3D9Hooking::Direct3D9::GetDeviceVTable(UINTX *pVTable) const
{
	if (vtable)
	{
		memcpy(pVTable, vtable, VTableElements * sizeof(UINTX));
		return true;
	}

	return false;
}
