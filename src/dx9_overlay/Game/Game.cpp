#include <Utils/Windows.h>
#include <Utils/Hook.h>
#include <Utils/Pattern.h>
#include <Utils/PipeServer.h>

#include "Game.h"
#include "Messagehandler.h"

#include "Rendering/Renderer.h"

#include <d3dx9.h>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>

#define DX9_VTABLE_RELEASE				0x02
#define DX9_VTABLE_PRESENT				0x11
#define DX9_VTABLE_RESET				0x10
#define DX9_VTABLE_PRESENTEX			0x79
#define DX9X_VTABLE_RESETEX				0x84
#define DX9_VTABLE_SWAPCHAIN_PRESENT	0x03

#define BIND(T) PaketHandler[PipeMessages::T] = std::bind(T, std::placeholders::_1, std::placeholders::_2);

Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *> g_presentHook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS *> g_resetHook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9EX, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *, DWORD> g_presentExHook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9EX, D3DPRESENT_PARAMETERS *, D3DDISPLAYMODEEX *> g_resetExHook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DSWAPCHAIN9, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *> g_swapchainPresentHook;

Renderer g_pRenderer;
bool g_bEnabled = false;

extern "C" __declspec(dllexport) void enable()
{
	g_bEnabled = true;
}

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

void initGame()
{
	HMODULE hMod = NULL;

	logging::add_file_log
	(
		keywords::file_name = "dx9_overlay.log",                                        /* file name pattern            */
		keywords::auto_flush = true,
		keywords::format = "[%TimeStamp%]: %Message%"                                 /* log record format            */
	);

	logging::core::get()->set_filter
	(
		logging::trivial::severity >= logging::trivial::info
	);

	while ((hMod = GetModuleHandle("d3d9.dll")) == NULL || g_bEnabled == false)
		Sleep(200);

	BOOST_LOG_TRIVIAL(info) << "Library enabled";

	BOOST_LOG_TRIVIAL(info) << "Acquiring VTable for Direct3DCreate9Ex...";

	WNDCLASSEX window_class;
	ZeroMemory(&window_class, sizeof(WNDCLASSEX));

	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = DefWindowProc;
	window_class.lpszClassName = "TempDirectXOverlayWindow";

	window_class.hInstance = GetModuleHandle(NULL);
	if (window_class.hInstance == NULL)
		BOOST_LOG_TRIVIAL(fatal) << "Could not get the instance handle";

	if (!RegisterClassEx(&window_class))
		BOOST_LOG_TRIVIAL(fatal) << "Could not get register the window class";

	HWND temp_window = CreateWindow(window_class.lpszClassName, "Temporary DirectX Overlay Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, window_class.hInstance, NULL);
	if (temp_window == NULL)
		BOOST_LOG_TRIVIAL(fatal) << "Could not get create the temporary window";

	LPVOID Direct3DCreate9Ex = (LPVOID)GetProcAddress(hMod, "Direct3DCreate9Ex");
	if (Direct3DCreate9Ex == NULL)
		BOOST_LOG_TRIVIAL(fatal) << "Could not locate the Direct3DCreate9 procedure entry point";

	IDirect3D9Ex *d3d9_ex;
	HRESULT error_code = ((HRESULT(WINAPI *)(UINT, IDirect3D9Ex **)) Direct3DCreate9Ex)(D3D_SDK_VERSION, &d3d9_ex);
	if (FAILED(error_code))
		BOOST_LOG_TRIVIAL(fatal) << "Could not create the DirectX 9 interface";

	D3DDISPLAYMODE display_mode;
	if (FAILED(d3d9_ex->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display_mode)))
		BOOST_LOG_TRIVIAL(fatal) << "Could not determine the current display mode";

	D3DPRESENT_PARAMETERS present_parameters;
	ZeroMemory(&present_parameters, sizeof(D3DPRESENT_PARAMETERS));
	present_parameters.Windowed = TRUE;
	present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	present_parameters.BackBufferFormat = display_mode.Format;

	IDirect3DDevice9Ex *d3d9_device_ex;
	error_code = d3d9_ex->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, temp_window, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT, &present_parameters, NULL, &d3d9_device_ex);
	if (FAILED(error_code))
		BOOST_LOG_TRIVIAL(fatal) << "Could not create the Direct3D 9 device";

#ifdef _M_IX86
	UINT32 *vtableEx = *((UINT32 **)d3d9_device_ex);
#else
	UINT64 *vtableEx = *((UINT64 **)d3d9_device_ex);
#endif

	BOOST_LOG_TRIVIAL(info) << "VTable acquired";


	BOOST_LOG_TRIVIAL(info) << "Acquiring VTable for IDirect3DSwapChain9...";

	IDirect3DSwapChain9 *swap_chain;
	if (FAILED(d3d9_device_ex->GetSwapChain(0, &swap_chain)))
		BOOST_LOG_TRIVIAL(fatal) << "Could not obtain the swap chain";

#ifdef _M_IX86
	UINT32 *swapchain_vtable = *((UINT32 **)swap_chain);
#else
	UINT64 *swapchain_vtable = *((UINT64 **)swap_chain);
#endif

	BOOST_LOG_TRIVIAL(info) << "VTable acquired";


	BOOST_LOG_TRIVIAL(info) << "Releasing temporary objects";

	swap_chain->Release();
	d3d9_device_ex->Release();
	d3d9_ex->Release();

	if (!DestroyWindow(temp_window))
		BOOST_LOG_TRIVIAL(fatal) << "Could not release the temporary window";

	if (!UnregisterClass(window_class.lpszClassName, window_class.hInstance))
		BOOST_LOG_TRIVIAL(fatal) << "Could not release the window class";


	DWORD *vtbl = 0;
	DWORD dwDevice = findPattern((DWORD) hMod, 0x128000, (PBYTE)"\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86", "xx????xx????xx");
	memcpy(&vtbl, (void *) (dwDevice + 0x2), 4);
	

	BOOST_LOG_TRIVIAL(info) << "Initializing hook engine...";

	if (MH_Initialize() != MH_OK)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Couldn't initialize hook engine";
		return;
	}

	BOOST_LOG_TRIVIAL(info) << "Hook engine initialized";

	

	BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9::Present";

	g_presentHook.apply(vtbl[17], [](LPDIRECT3DDEVICE9 dev, CONST RECT * a1, CONST RECT * a2, HWND a3, CONST RGNDATA *a4) -> HRESULT
	{
		__asm pushad
		g_pRenderer.draw(dev);
		__asm popad

		return g_presentHook.callOrig(dev, a1, a2, a3, a4);
	});

	BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9::Reset";

	g_resetHook.apply(vtbl[16], [](LPDIRECT3DDEVICE9 dev, D3DPRESENT_PARAMETERS *pp) -> HRESULT
	{
		__asm pushad
		g_pRenderer.reset(dev);
		__asm popad

		return g_resetHook.callOrig(dev, pp);
	});

	BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9Ex::PresentEx";

	g_presentExHook.apply(vtableEx[DX9_VTABLE_PRESENTEX], [](LPDIRECT3DDEVICE9EX dev, CONST RECT * a1, CONST RECT * a2, HWND a3, CONST RGNDATA *a4, DWORD a5) -> HRESULT
	{
		__asm pushad
		g_pRenderer.draw(dev);
		__asm popad

		return g_presentExHook.callOrig(dev, a1, a2, a3, a4, a5);
	});

	BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9Ex::ResetEx";

	g_resetExHook.apply(vtableEx[DX9X_VTABLE_RESETEX], [](LPDIRECT3DDEVICE9EX dev, D3DPRESENT_PARAMETERS *pp, D3DDISPLAYMODEEX *ppp) -> HRESULT
	{
		__asm pushad
		g_pRenderer.reset(dev);
		__asm popad

		return g_resetExHook.callOrig(dev, pp, ppp);
	});

	BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DSwapChain9::Present";

	g_swapchainPresentHook.apply(swapchain_vtable[DX9_VTABLE_SWAPCHAIN_PRESENT], [](LPDIRECT3DSWAPCHAIN9 swap_chain, CONST RECT * a1, CONST RECT * a2, HWND a3, CONST RGNDATA *a4) -> HRESULT
	{
		IDirect3DDevice9 *device;
		HRESULT error_code = swap_chain->GetDevice(&device);
		if (FAILED(error_code))
			return error_code;

		__asm pushad
		g_pRenderer.draw(device);
		__asm popad

		return g_swapchainPresentHook.callOrig(swap_chain, a1, a2, a3, a4);
	});


	typedef std::map<PipeMessages, std::function<void(Serializer&, Serializer&)> > MessagePaketHandler;
	MessagePaketHandler PaketHandler;

	BIND(TextCreate);
	BIND(TextDestroy);
	BIND(TextSetShadow);
	BIND(TextSetShown);
	BIND(TextSetColor);
	BIND(TextSetPos);
	BIND(TextSetString);
	BIND(TextUpdate);

	BIND(BoxCreate);
	BIND(BoxDestroy);
	BIND(BoxSetShown);
	BIND(BoxSetBorder);
	BIND(BoxSetBorderColor);
	BIND(BoxSetColor);
	BIND(BoxSetHeight);
	BIND(BoxSetPos);
	BIND(BoxSetWidth);

	BIND(LineCreate);
	BIND(LineDestroy);
	BIND(LineSetShown);
	BIND(LineSetColor);
	BIND(LineSetWidth);
	BIND(LineSetPos);

	BIND(ImageCreate);
	BIND(ImageDestroy);
	BIND(ImageSetShown);
	BIND(ImageSetAlign);
	BIND(ImageSetPos);
	BIND(ImageSetRotation);

	BIND(DestroyAllVisual);
	BIND(ShowAllVisual);
	BIND(HideAllVisual);

	BIND(GetFrameRate);
	BIND(GetScreenSpecs);

	BIND(SetCalculationRatio);
	BIND(SetOverlayPriority);

	new PipeServer([&](Serializer& serializerIn, Serializer& serializerOut)
	{
		SERIALIZATION_READ(serializerIn, PipeMessages, eMessage);

		try
		{
			auto it = PaketHandler.find(eMessage);
			if (it == PaketHandler.end())
				return;

			if (!PaketHandler[eMessage])
				return;

			PaketHandler[eMessage](serializerIn, serializerOut);
		}
		catch (...)
		{
		}
	});

	while (true){
		Sleep(100);
	}
}
