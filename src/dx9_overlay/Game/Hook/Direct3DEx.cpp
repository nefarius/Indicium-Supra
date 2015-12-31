#include "Direct3DEx.h"
#include <boost/log/trivial.hpp>


Direct3DEx::Direct3DEx() : vtable(nullptr), d3d9_ex(nullptr), d3d9_device_ex(nullptr)
{
	BOOST_LOG_TRIVIAL(info) << "Acquiring VTable for Direct3DCreate9Ex...";

	HMODULE hMod = GetModuleHandle("d3d9.dll");

	if (hMod == NULL)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Could not get the handle to d3d9.dll";
		return;
	}
	
	ZeroMemory(&window_class, sizeof(WNDCLASSEX));

	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = DefWindowProc;
	window_class.lpszClassName = "TempDirectXOverlayWindow";

	window_class.hInstance = GetModuleHandle(NULL);
	if (window_class.hInstance == NULL)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Could not get the instance handle";
		return;
	}

	if (!RegisterClassEx(&window_class))
	{
		BOOST_LOG_TRIVIAL(fatal) << "Could not get register the window class";
		return;
	}

	temp_window = CreateWindow(window_class.lpszClassName, "Temporary DirectX Overlay Window",
		WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, window_class.hInstance, NULL);
	if (temp_window == NULL)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Could not get create the temporary window";
		return;
	}

	LPVOID Direct3DCreate9Ex = (LPVOID)GetProcAddress(hMod, "Direct3DCreate9Ex");
	if (Direct3DCreate9Ex == NULL)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Could not locate the Direct3DCreate9 procedure entry point";
		return;
	}

	HRESULT error_code = ((HRESULT(WINAPI *)(UINT, IDirect3D9Ex **)) Direct3DCreate9Ex)(D3D_SDK_VERSION, &d3d9_ex);
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

	error_code = d3d9_ex->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, temp_window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT, &present_parameters, NULL, &d3d9_device_ex);
	if (FAILED(error_code))
	{
		BOOST_LOG_TRIVIAL(fatal) << "Could not create the Direct3D 9 device"; return;
		return;
	}

#ifdef _M_IX86
	vtable = *((UINT32 **)d3d9_device_ex);
#else
	vtable = *((UINT64 **)d3d9_device_ex);
#endif

	BOOST_LOG_TRIVIAL(info) << "VTable acquired";
}

Direct3DEx::~Direct3DEx()
{
	BOOST_LOG_TRIVIAL(info) << "Releasing temporary objects";

	if (d3d9_device_ex)
		d3d9_device_ex->Release();

	if (d3d9_ex)
		d3d9_ex->Release();

	if (!DestroyWindow(temp_window))
		BOOST_LOG_TRIVIAL(fatal) << "Could not release the temporary window";

	if (!UnregisterClass(window_class.lpszClassName, window_class.hInstance))
		BOOST_LOG_TRIVIAL(fatal) << "Could not release the window class";
}

#ifdef _M_IX86
bool Direct3DEx::GetVTableEx(UINT32 *pVTable) const
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