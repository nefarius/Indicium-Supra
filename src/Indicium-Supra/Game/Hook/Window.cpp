#include "Window.h"
#include <boost/log/trivial.hpp>


Window::Window() : temp_window(nullptr)
{
	ZeroMemory(&window_class, sizeof(WNDCLASSEX));

	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = DefWindowProc;
	window_class.lpszClassName = "TempDirectXOverlayWindow";

	window_class.hInstance = GetModuleHandle(nullptr);
	if (window_class.hInstance == nullptr)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Could not get the instance handle";
		return;
	}

	if (!RegisterClassEx(&window_class))
	{
		BOOST_LOG_TRIVIAL(fatal) << "Could not get register the window class";
		return;
	}

	temp_window = CreateWindow(window_class.lpszClassName, "Temporary DirectX Overlay Window",
		WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, window_class.hInstance, NULL);
	if (temp_window == nullptr)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Could not get create the temporary window";
		return;
	}
}

Window::~Window()
{
	if (!DestroyWindow(temp_window))
		BOOST_LOG_TRIVIAL(fatal) << "Could not release the temporary window";

	if (!UnregisterClass(window_class.lpszClassName, window_class.hInstance))
		BOOST_LOG_TRIVIAL(fatal) << "Could not release the window class";
}

HWND Window::GetWindowHandle() const
{
	return temp_window;
}
