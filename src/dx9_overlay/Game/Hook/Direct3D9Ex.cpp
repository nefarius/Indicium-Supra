#include "Direct3D9Ex.h"
#include <boost/log/trivial.hpp>


Direct3D9Hooking::Direct3D9Ex::Direct3D9Ex() : vtable(nullptr), temp_window(nullptr), d3d9_ex(nullptr), d3d9_device_ex(nullptr)
{
	temp_window = new Window();

	BOOST_LOG_TRIVIAL(info) << "Acquiring VTable for Direct3DCreate9Ex...";

	auto hMod = GetModuleHandle("d3d9.dll");

	if (hMod == nullptr)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Could not get the handle to d3d9.dll";
		return;
	}

	auto Direct3DCreate9Ex = static_cast<LPVOID>(GetProcAddress(hMod, "Direct3DCreate9Ex"));
	if (Direct3DCreate9Ex == nullptr)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Could not locate the Direct3DCreate9 procedure entry point";
		return;
	}

	auto error_code = static_cast<HRESULT(WINAPI *)(UINT, IDirect3D9Ex **)>(Direct3DCreate9Ex)(D3D_SDK_VERSION, &d3d9_ex);
	if (FAILED(error_code))
	{
		BOOST_LOG_TRIVIAL(fatal) << "Could not create the DirectX 9 interface";
		return;
	}

	D3DDISPLAYMODE display_mode;
	if (FAILED(d3d9_ex->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display_mode)))
	{
		BOOST_LOG_TRIVIAL(fatal) << "Could not determine the current display mode";
		return;
	}

	D3DPRESENT_PARAMETERS present_parameters;
	ZeroMemory(&present_parameters, sizeof(D3DPRESENT_PARAMETERS));
	present_parameters.Windowed = TRUE;
	present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	present_parameters.BackBufferFormat = display_mode.Format;

	error_code = d3d9_ex->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, temp_window->GetWindowHandle(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT, &present_parameters, nullptr, &d3d9_device_ex);
	if (FAILED(error_code))
	{
		BOOST_LOG_TRIVIAL(fatal) << "Could not create the Direct3D 9 device";
		return;
	}

#ifdef _M_IX86
	vtable = *reinterpret_cast<UINT32 **>(d3d9_device_ex);
#else
	vtable = *((UINT64 **)d3d9_device_ex);
#endif

	BOOST_LOG_TRIVIAL(info) << "VTable acquired";
}

Direct3D9Hooking::Direct3D9Ex::~Direct3D9Ex()
{
	BOOST_LOG_TRIVIAL(info) << "Releasing temporary objects";

	if (d3d9_device_ex)
		d3d9_device_ex->Release();

	if (d3d9_ex)
		d3d9_ex->Release();

	if (temp_window)
		delete temp_window;
}

#ifdef _M_IX86
bool Direct3D9Hooking::Direct3D9Ex::GetVTableEx(UINT32 *pVTable) const
{
	if (vtable)
	{
		memcpy(pVTable, vtable, VTableElements * sizeof(UINT32));
		return true;
	}

	return false;
}
#else
bool CDirect3DEx::GetVTableEx(UINT64 *pVTable)
{
	if (vtable)
	{
		memcpy(pVTable, vtable, VTableElements * sizeof(UINT64));
		return true;
	}

	return false;
}
#endif