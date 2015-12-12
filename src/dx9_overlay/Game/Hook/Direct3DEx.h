#pragma once
#include <d3dx9.h>
#include <Utils/Windows.h>
#include <boost/log/trivial.hpp>

class CDirect3DEx
{
private:
#ifdef _M_IX86
	UINT32 *vtable;
#else
	UINT64 *vtable;
#endif
	WNDCLASSEX window_class;
	HWND temp_window;
	IDirect3D9Ex *d3d9_ex;
	IDirect3DDevice9Ex *d3d9_device_ex;
public:
	CDirect3DEx();
	~CDirect3DEx();

#ifdef _M_IX86
	UINT32 *GetVTableEx();
#else
	UINT64 *GetVTableEx();
#endif
};

