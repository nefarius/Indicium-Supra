#pragma once
#include <d3dx9.h>
#include <Window.h>

class Direct3DEx
{
#ifdef _M_IX86
	UINT32 *vtable;
#else
	UINT64 *vtable;
#endif
	Window *temp_window;
	IDirect3D9Ex *d3d9_ex;
	IDirect3DDevice9Ex *d3d9_device_ex;
public:
	Direct3DEx();
	~Direct3DEx();
	static const int VTableElements = 134;

#ifdef _M_IX86
	bool GetVTableEx(UINT32 *pVTable) const;
#else
	bool GetVTableEx(UINT64 *pVTable);
#endif
};

