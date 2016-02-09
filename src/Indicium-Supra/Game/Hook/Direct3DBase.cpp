#include "Direct3DBase.h"


Direct3DHooking::Direct3DBase::Direct3DBase(): vtable(nullptr)
{
	temp_window = new Window();
}


Direct3DHooking::Direct3DBase::~Direct3DBase()
{
	if (temp_window)
		delete temp_window;
}

