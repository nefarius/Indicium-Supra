#pragma once
#include <d3d9.h>
#include <Window.h>

class Direct3D
{
#ifdef _M_IX86
	UINT32 *vtable;
#else
	UINT64 *vtable;
#endif
	Window *temp_window;
	IDirect3D9 *d3d9;
	IDirect3DDevice9 *d3d9_device;
public:
	Direct3D();
	~Direct3D();
	static const int VTableElements = 119;

#ifdef _M_IX86
	bool GetVTable(UINT32 *pVTable) const;
#else
	bool GetVTable(UINT64 *pVTable);
#endif
};

