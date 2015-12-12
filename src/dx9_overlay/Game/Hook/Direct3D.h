#pragma once
#include <d3d9.h>
#include <Utils/Windows.h>
#include <boost/log/trivial.hpp>

class CDirect3D
{
private:
#ifdef _M_IX86
	UINT32 *vtable;
#else
	UINT64 *vtable;
#endif
	WNDCLASSEX window_class;
	HWND temp_window;
	IDirect3D9 *d3d9;
	IDirect3DDevice9 *d3d9_device;
public:
	CDirect3D();
	~CDirect3D();
	static const int VTableElements = 119;

#ifdef _M_IX86
	bool GetVTable(UINT32 *pVTable);
#else
	bool GetVTable(UINT64 *pVTable);
#endif
};

