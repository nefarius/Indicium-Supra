#pragma once

#include "intarch.h"
#include "Window.h"

#include <Poco/Logger.h>
using Poco::Logger;

namespace Direct3DHooking
{
	class Direct3DBase
	{
	protected:
		UINTX* vtable;
		Window* temp_window;
        Logger& logger = Logger::get(typeid(this).name());
	public:
		Direct3DBase();
		virtual ~Direct3DBase();

		virtual bool GetDeviceVTable(UINTX* pVTable) const = 0;
	};
}

