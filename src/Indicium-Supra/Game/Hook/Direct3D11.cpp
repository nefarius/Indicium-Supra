#include "Direct3D11.h"


Direct3D11Hooking::Direct3D11::Direct3D11()
{
}


Direct3D11Hooking::Direct3D11::~Direct3D11()
{
}

bool Direct3D11Hooking::Direct3D11::GetDeviceVTable(UINTX* pVTable) const
{
	// TODO: implement
	if (vtable)
	{
		memcpy(pVTable, vtable,  sizeof(UINTX));
		return true;
	}

	return false;
}
