#include "Window.h"
#include "Utils/Misc.h"


Window::Window() : temp_window(nullptr)
{
    auto& logger = Logger::get(LOG_REGION());

	ZeroMemory(&window_class, sizeof(WNDCLASSEX));

	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = DefWindowProc;
	window_class.lpszClassName = "TempDirectXOverlayWindow";

	window_class.hInstance = GetModuleHandle(nullptr);
	if (window_class.hInstance == nullptr)
	{
		logger.fatal("Could not get the instance handle");
		return;
	}

	if (!RegisterClassEx(&window_class))
	{
        logger.fatal("Could not get register the window class");
		return;
	}

	temp_window = CreateWindow(window_class.lpszClassName, "Temporary DirectX Overlay Window",
		WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, window_class.hInstance, NULL);
	if (temp_window == nullptr)
	{
        logger.fatal("Could not get create the temporary window");
		return;
	}
}

Window::~Window()
{
    auto& logger = Logger::get(LOG_REGION());

	if (!DestroyWindow(temp_window))
        logger.fatal("Could not release the temporary window");

	if (!UnregisterClass(window_class.lpszClassName, window_class.hInstance))
        logger.fatal("Could not release the window class");
}

HWND Window::GetWindowHandle() const
{
	return temp_window;
}
