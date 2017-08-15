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
        Window temp_window;
    public:
        Direct3DBase() : vtable(nullptr) {}
        virtual ~Direct3DBase() {}

        virtual bool GetDeviceVTable(UINTX* pVTable) const = 0;
    };
}

