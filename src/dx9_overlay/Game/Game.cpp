#include <Utils/Windows.h>
#include <Utils/Hook.h>
#include <Utils/PipeServer.h>

#include "Game.h"
#include "Messagehandler.h"

#include "Rendering/Renderer.h"

#include <d3dx9.h>

#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <Psapi.h>
#pragma comment(lib, "psapi.lib")

#include <Game/Hook/Direct3D9.h>
#include <Game/Hook/Direct3D9Ex.h>
#include <Game/Hook/Direct3D10.h>
#include <Game/Hook/DirectInput8.h>


#define BIND(T) PaketHandler[PipeMessages::T] = std::bind(T, std::placeholders::_1, std::placeholders::_2);

// D3D9
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *> g_present9Hook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS *> g_reset9Hook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9> g_endScene9Hook;

// D3D9Ex
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9EX, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *, DWORD> g_present9ExHook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9EX, D3DPRESENT_PARAMETERS *, D3DDISPLAYMODEEX *> g_reset9ExHook;

// D3D10
Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, UINT, UINT> g_swapChainPresent10Hook;
Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, const DXGI_MODE_DESC*> g_swapChainResizeTarget10Hook;

//DInput8
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECTINPUTDEVICE8> g_acquire8Hook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECTINPUTDEVICE8, DWORD, LPDIDEVICEOBJECTDATA, LPDWORD, DWORD> g_getDeviceData8Hook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECTINPUTDEVICE8, LPDIDEVICEINSTANCE> g_getDeviceInfo8Hook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECTINPUTDEVICE8, DWORD, LPVOID> g_getDeviceState8Hook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECTINPUTDEVICE8, LPDIDEVICEOBJECTINSTANCE, DWORD, DWORD> g_getObjectInfo8Hook;


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


template <typename T>
inline MH_STATUS MH_CreateHookApiEx(
	LPCWSTR pszModule, LPCSTR pszProcName, LPVOID pDetour, T** ppOriginal)
{
	return MH_CreateHookApi(
		pszModule, pszProcName, pDetour, reinterpret_cast<LPVOID*>(ppOriginal));
}

/*************************************************************************************/
#include <hidsdi.h>

typedef BOOLEAN(WINAPI* tHidD_GetInputReport)(
	_In_  HANDLE HidDeviceObject,
	_Out_ PVOID  ReportBuffer,
	_In_  ULONG  ReportBufferLength
	);
tHidD_GetInputReport OriginalHidD_GetInputReport = nullptr;

BOOLEAN WINAPI DetourHidD_GetInputReport(
	_In_  HANDLE HidDeviceObject,
	_Out_ PVOID  ReportBuffer,
	_In_  ULONG  ReportBufferLength
	);

typedef NTSTATUS (WINAPI* tHidP_GetData)(
	_In_    HIDP_REPORT_TYPE     ReportType,
	_Out_   PHIDP_DATA           DataList,
	_Inout_ PULONG               DataLength,
	_In_    PHIDP_PREPARSED_DATA PreparsedData,
	_In_    PCHAR                Report,
	_In_    ULONG                ReportLength
	);
tHidP_GetData OriginalHidP_GetData = nullptr;

NTSTATUS WINAPI DetourHidP_GetData(
	_In_    HIDP_REPORT_TYPE     ReportType,
	_Out_   PHIDP_DATA           DataList,
	_Inout_ PULONG               DataLength,
	_In_    PHIDP_PREPARSED_DATA PreparsedData,
	_In_    PCHAR                Report,
	_In_    ULONG                ReportLength
	);

typedef BOOLEAN (WINAPI* tHidD_GetPreparsedData)(
	_In_  HANDLE               HidDeviceObject,
	_Out_ PHIDP_PREPARSED_DATA *PreparsedData
	);
tHidD_GetPreparsedData OriginalHidD_GetPreparsedData = nullptr;

BOOLEAN WINAPI DetourHidD_GetPreparsedData(
	_In_  HANDLE               HidDeviceObject,
	_Out_ PHIDP_PREPARSED_DATA *PreparsedData
	);
/*************************************************************************************/

void initGame()
{
	HMODULE hMod = nullptr;
	bool d3d9_available, d3d9ex_available, d3d10_available;

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

	auto sz_ProcName = static_cast<LPSTR>(malloc(MAX_PATH + 1));
	GetProcessImageFileName(GetCurrentProcess(), sz_ProcName, MAX_PATH);
	BOOST_LOG_TRIVIAL(info) << "Library loaded into " << sz_ProcName;
	free(sz_ProcName);

#ifdef TEST
	while ((hMod = GetModuleHandle("d3d9.dll")) == nullptr || g_bEnabled == false)
		Sleep(200);
#endif

	BOOST_LOG_TRIVIAL(info) << "Library enabled";

#ifdef _M_IX86
	UINT32 vtable[Direct3D9Hooking::Direct3D9::VTableElements] = { 0 };
	UINT32 vtableEx[Direct3D9Hooking::Direct3D9Ex::VTableElements] = { 0 };
	UINT32 vtable10SwapChain[Direct3D10Hooking::Direct3D10::SwapChainVTableElements] = { 0 };
#else
	UINT64 vtable[Direct3D9Hooking::Direct3D9::VTableElements] = { 0 };
	UINT64 vtableEx[Direct3D9Hooking::Direct3D9Ex::VTableElements] = { 0 };
	UINT64 vtable10SwapChain[Direct3D10Hooking::Direct3D10::SwapChainVTableElements] = { 0 };
	UINT64 vtable8[DirectInput8Hooking::DirectInput8::VTableElements] = { 0 };
#endif

	// get VTable for Direct3DCreate9
	{
		Direct3D9Hooking::Direct3D9 d3d;
		d3d9_available = d3d.GetVTable(vtable);

		if (!d3d9_available)
		{
			BOOST_LOG_TRIVIAL(error) << "Couldn't get VTable for Direct3DCreate9";
		}
	}

	// get VTable for Direct3DCreate9Ex
	{
		Direct3D9Hooking::Direct3D9Ex d3dEx;
		d3d9ex_available = d3dEx.GetVTableEx(vtableEx);

		if (!d3d9ex_available)
		{
			BOOST_LOG_TRIVIAL(error) << "Couldn't get VTable for Direct3DCreate9Ex";
		}
	}

	// get VTable for IDXGISwapChain
	{
		Direct3D10Hooking::Direct3D10 d3d10;
		d3d10_available = d3d10.GetSwapChainVTable(vtable10SwapChain);

		if (!d3d10_available)
		{
			BOOST_LOG_TRIVIAL(error) << "Couldn't get VTable for IDXGISwapChain";
		}
	}

	// Dinput8
	{
		DirectInput8Hooking::DirectInput8 di8;
		di8.GetVTable(vtable8);

		if (!vtable8)
		{
			BOOST_LOG_TRIVIAL(error) << "Couldn't get VTable for DirectInput8";
		}
	}

	BOOST_LOG_TRIVIAL(info) << "Initializing hook engine...";

	if (MH_Initialize() != MH_OK)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Couldn't initialize hook engine";
		return;
	}

	BOOST_LOG_TRIVIAL(info) << "Hook engine initialized";

	if (d3d9_available)
	{
		BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9::Present";

		g_present9Hook.apply(vtable[Direct3D9Hooking::Present], [](LPDIRECT3DDEVICE9 dev, CONST RECT* a1, CONST RECT* a2, HWND a3, CONST RGNDATA* a4) -> HRESULT
		{
			g_bIsUsingPresent = true;

			g_pRenderer.draw(dev);

			return g_present9Hook.callOrig(dev, a1, a2, a3, a4);
		});

		BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9::Reset";

		g_reset9Hook.apply(vtable[Direct3D9Hooking::Reset], [](LPDIRECT3DDEVICE9 dev, D3DPRESENT_PARAMETERS* pp) -> HRESULT
		{
			g_pRenderer.reset(dev);

			return g_reset9Hook.callOrig(dev, pp);
		});

		BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9::EndScene";

		g_endScene9Hook.apply(vtable[Direct3D9Hooking::EndScene], [](LPDIRECT3DDEVICE9 dev) -> HRESULT
		{
			if (!g_bIsUsingPresent)
			{
				g_pRenderer.draw(dev);
			}

			return g_endScene9Hook.callOrig(dev);
		});
	}

	if (d3d9ex_available)
	{
		BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9Ex::PresentEx";

		g_present9ExHook.apply(vtableEx[Direct3D9Hooking::PresentEx], [](LPDIRECT3DDEVICE9EX dev, CONST RECT* a1, CONST RECT* a2, HWND a3, CONST RGNDATA* a4, DWORD a5) -> HRESULT
		{
			g_bIsUsingPresent = true;

			g_pRenderer.draw(dev);

			return g_present9ExHook.callOrig(dev, a1, a2, a3, a4, a5);
		});

		BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9Ex::ResetEx";

		g_reset9ExHook.apply(vtableEx[Direct3D9Hooking::ResetEx], [](LPDIRECT3DDEVICE9EX dev, D3DPRESENT_PARAMETERS* pp, D3DDISPLAYMODEEX* ppp) -> HRESULT
		{
			g_pRenderer.reset(dev);

			return g_reset9ExHook.callOrig(dev, pp, ppp);
		});
	}

	if (d3d10_available)
	{
		BOOST_LOG_TRIVIAL(info) << "Hooking IDXGISwapChain::Present";

		g_swapChainPresent10Hook.apply(vtable10SwapChain[Direct3D10Hooking::Present], [](IDXGISwapChain *chain, UINT SyncInterval, UINT Flags) -> HRESULT
		{
			BOOST_LOG_TRIVIAL(info) << "IDXGISwapChain::Present called";

			return g_swapChainPresent10Hook.callOrig(chain, SyncInterval, Flags);
		});

		BOOST_LOG_TRIVIAL(info) << "Hooking IDXGISwapChain::ResizeTarget";

		g_swapChainResizeTarget10Hook.apply(vtable10SwapChain[Direct3D10Hooking::ResizeTarget], [](IDXGISwapChain *chain, const DXGI_MODE_DESC *pNewTargetParameters) -> HRESULT
		{
			BOOST_LOG_TRIVIAL(info) << "IDXGISwapChain::ResizeTarget called";

			return g_swapChainResizeTarget10Hook.callOrig(chain, pNewTargetParameters);
		});
	}

	if (vtable8)
	{
		BOOST_LOG_TRIVIAL(info) << "Hooking IDirectInputDevice8::Acquire";

		g_acquire8Hook.apply(vtable8[DirectInput8Hooking::Acquire], [](LPDIRECTINPUTDEVICE8 dev) -> HRESULT
		{
			BOOST_LOG_TRIVIAL(info) << "IDirectInputDevice8::Acquire called";

			return g_acquire8Hook.callOrig(dev);
		});

		BOOST_LOG_TRIVIAL(info) << "Hooking IDirectInputDevice8::GetDeviceData";

		g_getDeviceData8Hook.apply(vtable8[DirectInput8Hooking::GetDeviceData], [](LPDIRECTINPUTDEVICE8 dev, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags) -> HRESULT
		{
			BOOST_LOG_TRIVIAL(info) << "IDirectInputDevice8::GetDeviceData called";

			return g_getDeviceData8Hook.callOrig(dev, cbObjectData, rgdod, pdwInOut, dwFlags);
		});

		BOOST_LOG_TRIVIAL(info) << "Hooking IDirectInputDevice8::GetDeviceInfo ";

		g_getDeviceInfo8Hook.apply(vtable8[DirectInput8Hooking::GetDeviceInfo], [](LPDIRECTINPUTDEVICE8 dev, LPDIDEVICEINSTANCE pdidi) -> HRESULT
		{
			BOOST_LOG_TRIVIAL(info) << "IDirectInputDevice8::GetDeviceInfo  called";

			return g_getDeviceInfo8Hook.callOrig(dev, pdidi);
		});

		BOOST_LOG_TRIVIAL(info) << "Hooking IDirectInputDevice8::GetDeviceState";

		g_getDeviceState8Hook.apply(vtable8[DirectInput8Hooking::GetDeviceState], [](LPDIRECTINPUTDEVICE8 dev, DWORD cbData, LPVOID lpvData) -> HRESULT
		{
			BOOST_LOG_TRIVIAL(info) << "IDirectInputDevice8::GetDeviceState called";
			
			return g_getDeviceState8Hook.callOrig(dev, cbData, lpvData);
		});

		BOOST_LOG_TRIVIAL(info) << "Hooking IDirectInputDevice8::GetObjectInfo";

		g_getObjectInfo8Hook.apply(vtable8[DirectInput8Hooking::GetObjectInfo], [](LPDIRECTINPUTDEVICE8 dev, LPDIDEVICEOBJECTINSTANCE pdidoi, DWORD dwObj, DWORD dwHow) -> HRESULT
		{
			BOOST_LOG_TRIVIAL(info) << "IDirectInputDevice8::GetObjectInfo called";

			return g_getObjectInfo8Hook.callOrig(dev, pdidoi, dwObj, dwHow);
		});
	}

	/*********************************************************************************************************************************************/
	if (MH_CreateHookApiEx(L"hid", "HidD_GetInputReport", &DetourHidD_GetInputReport, &OriginalHidD_GetInputReport) != MH_OK)
	{
		BOOST_LOG_TRIVIAL(error) << "Couldn't create HidD_GetInputReport hook";
	}

	if (MH_EnableHook(GetProcAddress(GetModuleHandle("hid"), "HidD_GetInputReport")) != MH_OK)
	{
		BOOST_LOG_TRIVIAL(error) << "Couldn't enable HidD_GetInputReport hook";
	}

	/*********************************************************************************************************************************************/
	if (MH_CreateHookApiEx(L"hid", "HidP_GetData", &DetourHidP_GetData, &OriginalHidP_GetData) != MH_OK)
	{
		BOOST_LOG_TRIVIAL(error) << "Couldn't create HidD_GetInputReport hook";
	}

	if (MH_EnableHook(GetProcAddress(GetModuleHandle("hid"), "HidP_GetData")) != MH_OK)
	{
		BOOST_LOG_TRIVIAL(error) << "Couldn't enable HidP_GetData hook";
	}

	/*********************************************************************************************************************************************/
	if (MH_CreateHookApiEx(L"hid", "HidD_GetPreparsedData", &DetourHidD_GetPreparsedData, &OriginalHidD_GetPreparsedData) != MH_OK)
	{
		BOOST_LOG_TRIVIAL(error) << "Couldn't create HidD_GetPreparsedData hook";
	}

	if (MH_EnableHook(GetProcAddress(GetModuleHandle("hid"), "HidD_GetPreparsedData")) != MH_OK)
	{
		BOOST_LOG_TRIVIAL(error) << "Couldn't enable HidD_GetPreparsedData hook";
	}




	typedef std::map<PipeMessages, std::function<void(Serializer&, Serializer&)>> MessagePaketHandler;
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
	BIND(ImageSetScale);

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

	// block this thread infinitely
	WaitForSingleObject(INVALID_HANDLE_VALUE, INFINITE);
}

BOOLEAN WINAPI DetourHidD_GetInputReport(
	_In_  HANDLE HidDeviceObject,
	_Out_ PVOID  ReportBuffer,
	_In_  ULONG  ReportBufferLength
	)
{
	BOOST_LOG_TRIVIAL(info) << "DetourHidD_GetInputReport called";

	return OriginalHidD_GetInputReport(HidDeviceObject, ReportBuffer, ReportBufferLength);
}

NTSTATUS WINAPI DetourHidP_GetData(
	_In_    HIDP_REPORT_TYPE     ReportType,
	_Out_   PHIDP_DATA           DataList,
	_Inout_ PULONG               DataLength,
	_In_    PHIDP_PREPARSED_DATA PreparsedData,
	_In_    PCHAR                Report,
	_In_    ULONG                ReportLength
	)
{
	BOOST_LOG_TRIVIAL(info) << "DetourHidP_GetData called";

	return OriginalHidP_GetData(ReportType, DataList, DataLength, PreparsedData, Report, ReportLength);
}

BOOLEAN WINAPI DetourHidD_GetPreparsedData(
	_In_  HANDLE               HidDeviceObject,
	_Out_ PHIDP_PREPARSED_DATA *PreparsedData
	)
{
	BOOST_LOG_TRIVIAL(info) << "DetourHidD_GetPreparsedData called";

	return OriginalHidD_GetPreparsedData(HidDeviceObject, PreparsedData);
}
