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
#include <boost/log/utility/setup/common_attributes.hpp>

#include <Psapi.h>
#pragma comment(lib, "psapi.lib")

#include <Game/Hook/Direct3D.h>
#include <Game/Hook/Direct3DEx.h>

#define DX9_VTABLE_RELEASE				0x02
#define DX9_VTABLE_PRESENT				0x11
#define DX9_VTABLE_RESET				0x10
#define DX9_VTABLE_PRESENTEX			0x79
#define DX9_VTABLE_RESETEX				0x84
#define DX9_VTABLE_SWAPCHAIN_PRESENT	0x03
#define DX9_VTABLE_ENDSCENE				0x2A

#define BIND(T) PaketHandler[PipeMessages::T] = std::bind(T, std::placeholders::_1, std::placeholders::_2);

Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *> g_presentHook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS *> g_resetHook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9EX, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *, DWORD> g_presentExHook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9EX, D3DPRESENT_PARAMETERS *, D3DDISPLAYMODEEX *> g_resetExHook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DSWAPCHAIN9, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *> g_swapchainPresentHook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9> g_endSceneHook;

Renderer g_pRenderer;
bool g_bEnabled = false;
bool g_bIsUsingPresent = false;

extern "C" __declspec(dllexport) void enable()
{
	g_bEnabled = true;
}

namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;


void initGame()
{
	HMODULE hMod = NULL;

	logging::add_common_attributes();

	logging::add_file_log
	(
		keywords::file_name = "dx9_overlay.log",
		keywords::auto_flush = true,
		keywords::format = "[%TimeStamp%]: %Message%"
	);

	logging::core::get()->set_filter
	(
		logging::trivial::severity >= logging::trivial::info
	);

	LPSTR sz_ProcName = (LPSTR) malloc(MAX_PATH + 1);
	GetProcessImageFileName(GetCurrentProcess(), sz_ProcName, MAX_PATH);
	BOOST_LOG_TRIVIAL(info) << "Library loaded into " << sz_ProcName;
	free(sz_ProcName);

	while ((hMod = GetModuleHandle("d3d9.dll")) == NULL || g_bEnabled == false)
		Sleep(200);

	BOOST_LOG_TRIVIAL(info) << "Library enabled";

#ifdef _M_IX86
	UINT32 vtable[CDirect3D::VTableElements] = { };
	UINT32 vtableEx[CDirect3DEx::VTableElements] = {};
#else
	UINT64 vtable[CDirect3D::VTableElements] = { };
	UINT64 vtableEx[CDirect3DEx::VTableElements] = { };
#endif

	{
		CDirect3D d3d;
		d3d.GetVTable(vtable);
	}

	{
		CDirect3DEx d3dEx;
		d3dEx.GetVTableEx(vtableEx);
	}

#ifdef SWAPCHAIN
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
#endif

	

	BOOST_LOG_TRIVIAL(info) << "Initializing hook engine...";

	if (MH_Initialize() != MH_OK)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Couldn't initialize hook engine";
		return;
	}

	BOOST_LOG_TRIVIAL(info) << "Hook engine initialized";

	
	if (vtable)
	{
		BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9::Present";

		g_presentHook.apply(vtable[DX9_VTABLE_PRESENT], [](LPDIRECT3DDEVICE9 dev, CONST RECT * a1, CONST RECT * a2, HWND a3, CONST RGNDATA *a4) -> HRESULT
		{
			g_bIsUsingPresent = true;

			__asm pushad
			g_pRenderer.draw(dev);
			__asm popad

			return g_presentHook.callOrig(dev, a1, a2, a3, a4);
		});

		BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9::Reset";

		g_resetHook.apply(vtable[DX9_VTABLE_RESET], [](LPDIRECT3DDEVICE9 dev, D3DPRESENT_PARAMETERS *pp) -> HRESULT
		{
			static UINT32 counter = 0;
			static BOOL skip = FALSE;

			if (!skip || counter++ == 100)
			{
				skip = TRUE;
				BOOST_LOG_TRIVIAL(info) << "IDirect3DDevice9::Reset is used by process";
			}

			__asm pushad
			g_pRenderer.reset(dev);
			__asm popad

			return g_resetHook.callOrig(dev, pp);
		});

		BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9::EndScene";

		g_endSceneHook.apply(vtable[DX9_VTABLE_ENDSCENE], [](LPDIRECT3DDEVICE9 dev) -> HRESULT
		{
			if (!g_bIsUsingPresent)
			{
				__asm pushad
				g_pRenderer.draw(dev);
				__asm popad
			}

			return g_endSceneHook.callOrig(dev);
		});
	}

	if (vtableEx)
	{
		BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9Ex::PresentEx";

		g_presentExHook.apply(vtableEx[DX9_VTABLE_PRESENTEX], [](LPDIRECT3DDEVICE9EX dev, CONST RECT * a1, CONST RECT * a2, HWND a3, CONST RGNDATA *a4, DWORD a5) -> HRESULT
		{
			g_bIsUsingPresent = true;

			__asm pushad
			g_pRenderer.draw(dev);
			__asm popad

			return g_presentExHook.callOrig(dev, a1, a2, a3, a4, a5);
		});

		BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9Ex::ResetEx";

		g_resetExHook.apply(vtableEx[DX9_VTABLE_RESETEX], [](LPDIRECT3DDEVICE9EX dev, D3DPRESENT_PARAMETERS *pp, D3DDISPLAYMODEEX *ppp) -> HRESULT
		{
			static UINT32 counter = 0;
			static BOOL skip = FALSE;

			if (!skip || counter++ == 100)
			{
				skip = TRUE;
				BOOST_LOG_TRIVIAL(info) << "IDirect3DDevice9Ex::ResetEx is used by process";
			}

			__asm pushad
			g_pRenderer.reset(dev);
			__asm popad

			return g_resetExHook.callOrig(dev, pp, ppp);
		});
	}

#ifdef FIXME
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
#endif


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
