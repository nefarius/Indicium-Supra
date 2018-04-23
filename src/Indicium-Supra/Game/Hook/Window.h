#pragma once

#include <string>
#include <Poco/RefCountedObject.h>

class Window : public Poco::RefCountedObject
{
	WNDCLASSEX window_class;
	HWND temp_window;
    std::string window_class_name;
public:
	Window(std::string windowClassName);
	HWND windowHandle() const;

protected:
    ~Window();
};

