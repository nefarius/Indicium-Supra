#pragma once
#include <Utils/Windows.h>

#include <Poco/Logger.h>
using Poco::Logger;

class Window
{
	WNDCLASSEX window_class;
	HWND temp_window;
public:
	Window();
	~Window();
	HWND GetWindowHandle() const;
};

