#include "Direct3D9Ex.h"
#include <Poco/Exception.h>


Direct3D9Hooking::Direct3D9Ex::Direct3D9Ex() : Direct3DBase(), d3d9_ex(nullptr), d3d9_device_ex(nullptr)
{
    temp_window = new Window("TempDirect3D9ExOverlayWindow");

	auto hMod = GetModuleHandle("d3d9.dll");

	if (hMod == nullptr)
	{
		throw Poco::RuntimeException("Could not get the handle to d3d9.dll");
	}

	auto Direct3DCreate9Ex = static_cast<LPVOID>(GetProcAddress(hMod, "Direct3DCreate9Ex"));
	if (Direct3DCreate9Ex == nullptr)
	{
		throw Poco::RuntimeException("Could not locate the Direct3DCreate9 procedure entry point");
	}

	auto error_code = static_cast<HRESULT(WINAPI *)(UINT, IDirect3D9Ex**)>(Direct3DCreate9Ex)(D3D_SDK_VERSION, &d3d9_ex);
	if (FAILED(error_code))
	{
        throw Poco::RuntimeException("Could not create the DirectX 9 interface");
	}

	D3DDISPLAYMODE display_mode;
	if (FAILED(d3d9_ex->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display_mode)))
	{
        throw Poco::RuntimeException("Could not determine the current display mode");
	}

	D3DPRESENT_PARAMETERS present_parameters;
	ZeroMemory(&present_parameters, sizeof(D3DPRESENT_PARAMETERS));
	present_parameters.Windowed = TRUE;
	present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	present_parameters.BackBufferFormat = display_mode.Format;

	error_code = d3d9_ex->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, temp_window->windowHandle(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT, &present_parameters, nullptr, &d3d9_device_ex);
	if (FAILED(error_code))
	{
        throw Poco::RuntimeException("Could not create the Direct3D 9 device");
	}
}

std::vector<UINTX> Direct3D9Hooking::Direct3D9Ex::vtable() const
{
    UINTX vtbl[VTableElements];
    memcpy(vtbl, *reinterpret_cast<UINTX **>(d3d9_device_ex), VTableElements * sizeof(UINTX));
    return std::vector<UINTX>(vtbl, vtbl + sizeof vtbl / sizeof vtbl[0]);
}

Direct3D9Hooking::Direct3D9Ex::~Direct3D9Ex()
{
	if (d3d9_device_ex)
		d3d9_device_ex->Release();

	if (d3d9_ex)
		d3d9_ex->Release();
}
