#pragma once

#include "intarch.h"
#include "Window.h"

#include <Poco/Logger.h>
using Poco::Logger;

#include <Poco/AutoPtr.h>
#include <Poco/RefCountedObject.h>

using Poco::AutoPtr;
using Poco::RefCountedObject;

namespace Direct3DHooking
{
    class Direct3DBase : public RefCountedObject
    {
    protected:
        AutoPtr<Window> temp_window;
        virtual ~Direct3DBase() {}
    public:
        Direct3DBase() {}

        virtual std::vector<UINTX> vtable() const = 0;
    };
}

