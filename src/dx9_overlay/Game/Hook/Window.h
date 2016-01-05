#pragma once
#include <Utils/Windows.h>

class Window
{
	WNDCLASSEX window_class;
	HWND temp_window;
public:
	Window();
	~Window();
	HWND GetWindowHandle() const;
};

