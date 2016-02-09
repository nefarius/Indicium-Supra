#pragma once

#include "intarch.h"
#include "Window.h"

namespace Direct3DHooking
{
	class Direct3DBase
	{
	protected:
		UINTX* vtable;
		Window* temp_window;
	public:
		Direct3DBase();
		virtual ~Direct3DBase();

		virtual bool GetDeviceVTable(UINTX* pVTable) const = 0;
	};
}

