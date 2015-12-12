#include "Direct3D.h"


CDirect3D::CDirect3D()
{
	d3d9 = NULL;
	d3d9_device = NULL;
	vtable = NULL;

	BOOST_LOG_TRIVIAL(info) << "Acquiring VTable for Direct3DCreate9...";

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

	LPVOID Direct3DCreate9 = (LPVOID)GetProcAddress(hMod, "Direct3DCreate9");
	if (Direct3DCreate9 == NULL)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Could not locate the Direct3DCreate9 procedure entry point";
		return;
	}

	d3d9 = ((LPDIRECT3D9(WINAPI *)(UINT)) Direct3DCreate9)(D3D_SDK_VERSION);
	if (d3d9 == NULL)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Could not create the DirectX 9 interface";
		return;
	}

	D3DDISPLAYMODE display_mode;
	if (FAILED(d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display_mode)))
	{
		BOOST_LOG_TRIVIAL(fatal) << "Could not determine the current display mode";
		return;
	}

	D3DPRESENT_PARAMETERS present_parameters;
	ZeroMemory(&present_parameters, sizeof(D3DPRESENT_PARAMETERS));
	present_parameters.Windowed = TRUE;
	present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	present_parameters.BackBufferFormat = display_mode.Format;

	HRESULT error_code = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, temp_window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT, &present_parameters, &d3d9_device);
	if (FAILED(error_code))
	{
		BOOST_LOG_TRIVIAL(fatal) << "Could not create the Direct3D 9 device"; return;
		return;
	}

#ifdef _M_IX86
	vtable = *((UINT32 **)d3d9_device);
#else
	vtable = *((UINT64 **)d3d9_device);
#endif

	BOOST_LOG_TRIVIAL(info) << "VTable acquired";
}


CDirect3D::~CDirect3D()
{
	BOOST_LOG_TRIVIAL(info) << "Releasing temporary objects";

	if (d3d9_device)
		d3d9_device->Release();

	if (d3d9)
		d3d9->Release();

	if (!DestroyWindow(temp_window))
		BOOST_LOG_TRIVIAL(fatal) << "Could not release the temporary window";

	if (!UnregisterClass(window_class.lpszClassName, window_class.hInstance))
		BOOST_LOG_TRIVIAL(fatal) << "Could not release the window class";
}

#ifdef _M_IX86
bool CDirect3D::GetVTable(UINT32 *pVTable)
{
	if (vtable)
	{
		memcpy(pVTable, vtable, VTableElements * sizeof(UINT32));
		return true;
	}
	
	return false;
}
#else
bool CDirect3D::GetVTable(UINT64 *pVTable)
{
	if (vtable)
	{
		memcpy(pVTable, vtable, VTableElements * sizeof(UINT64));
		return true;
	}

	return false;
}
#endif