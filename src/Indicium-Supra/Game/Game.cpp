#include <Utils/Windows.h>
#include <Utils/Hook.h>
#include <Utils/PipeServer.h>

#include "Game.h"
#include "Messagehandler.h"

#include "Rendering/Renderer.h"

#include <Psapi.h>
#pragma comment(lib, "psapi.lib")

#include <Game/Hook/Direct3D9.h>
#include <Game/Hook/Direct3D9Ex.h>
#include <Game/Hook/DXGI.h>
#include <Game/Hook/Direct3D10.h>
#include <Game/Hook/Direct3D11.h>
#include <Game/Hook/DirectInput8.h>

#include <imgui/imgui_impl_dx9.h>
#include <imgui/imgui_impl_dx10.h>
#include <imgui/imgui_impl_dx11.h>


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

// D3D11
Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, UINT, UINT> g_swapChainPresent11Hook;
Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, const DXGI_MODE_DESC*> g_swapChainResizeTarget11Hook;

//DInput8
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECTINPUTDEVICE8> g_acquire8Hook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECTINPUTDEVICE8, DWORD, LPDIDEVICEOBJECTDATA, LPDWORD, DWORD> g_getDeviceData8Hook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECTINPUTDEVICE8, LPDIDEVICEINSTANCE> g_getDeviceInfo8Hook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECTINPUTDEVICE8, DWORD, LPVOID> g_getDeviceState8Hook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECTINPUTDEVICE8, LPDIDEVICEOBJECTINSTANCE, DWORD, DWORD> g_getObjectInfo8Hook;


Renderer g_pRenderer;
bool g_bEnabled = false;
bool g_bIsUsingPresent = false;
bool g_bIsImGuiInitialized = false;
HWND g_hWnd = nullptr;
tDefWindowProc OriginalDefWindowProc = nullptr;

extern "C" __declspec(dllexport) void enable()
{
	g_bEnabled = true;
}


void initGame()
{
	bool d3d9_available, d3d9ex_available, d3d10_available, d3d11_available, dinput8_available;
	
	auto sz_ProcName = static_cast<LPSTR>(malloc(MAX_PATH + 1));
	GetProcessImageFileName(GetCurrentProcess(), sz_ProcName, MAX_PATH);
	BOOST_LOG_TRIVIAL(info) << "Library loaded into " << sz_ProcName;
	free(sz_ProcName);

	BOOST_LOG_TRIVIAL(info) << "Library enabled";

	UINTX vtable9[Direct3D9Hooking::Direct3D9::VTableElements] = { 0 };
	UINTX vtable9Ex[Direct3D9Hooking::Direct3D9Ex::VTableElements] = { 0 };
	UINTX vtable10SwapChain[DXGIHooking::DXGI::SwapChainVTableElements] = { 0 };
	UINTX vtable11SwapChain[DXGIHooking::DXGI::SwapChainVTableElements] = { 0 };
	UINTX vtable8[DirectInput8Hooking::DirectInput8::VTableElements] = { 0 };

	// get VTable for Direct3DCreate9
	{
		Direct3D9Hooking::Direct3D9 d3d;
		d3d9_available = d3d.GetDeviceVTable(vtable9);

		if (!d3d9_available)
		{
			BOOST_LOG_TRIVIAL(error) << "Couldn't get VTable for Direct3DCreate9";
		}
	}

	// get VTable for Direct3DCreate9Ex
	{
		Direct3D9Hooking::Direct3D9Ex d3dEx;
		d3d9ex_available = d3dEx.GetDeviceVTable(vtable9Ex);

		if (!d3d9ex_available)
		{
			BOOST_LOG_TRIVIAL(error) << "Couldn't get VTable for Direct3DCreate9Ex";
		}
	}

	// get VTable for IDXGISwapChain (v10)
	{
		Direct3D10Hooking::Direct3D10 d3d10;
		d3d10_available = d3d10.GetSwapChainVTable(vtable10SwapChain);

		if (!d3d10_available)
		{
			BOOST_LOG_TRIVIAL(error) << "Couldn't get VTable for IDXGISwapChain";
		}
	}

	// get VTable for IDXGISwapChain (v11)
	{
		Direct3D11Hooking::Direct3D11 d3d11;
		d3d11_available = d3d11.GetSwapChainVTable(vtable11SwapChain);

		if (!d3d11_available)
		{
			BOOST_LOG_TRIVIAL(error) << "Couldn't get VTable for IDXGISwapChain";
		}
	}

	// Dinput8
	{
		DirectInput8Hooking::DirectInput8 di8;
		dinput8_available = di8.GetVTable(vtable8);

		if (!dinput8_available)
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

	HookDefWindowProc();

	if (d3d9_available)
	{
		HookDX9(vtable9);
	}

	if (d3d9ex_available)
	{
		HookDX9Ex(vtable9Ex);
	}

	if (d3d10_available)
	{
		HookDX10(vtable10SwapChain);
	}

	if (d3d11_available)
	{
		HookDX11(vtable11SwapChain);
	}

	if (dinput8_available)
	{
		HookDInput8(vtable8);
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

void logOnce(std::string message)
{
	BOOST_LOG_TRIVIAL(info) << message;
}

void HookDefWindowProc()
{
	BOOST_LOG_TRIVIAL(info) << "Hooking USER32!DefWindowProcA (ANSI)";

	if (MH_CreateHookApiEx(L"user32", "DefWindowProcA", &DetourDefWindowProc, &OriginalDefWindowProc) != MH_OK)
	{
		BOOST_LOG_TRIVIAL(error) << "Couldn't hook USER32!DefWindowProcA (ANSI)";
		return;
	}

	BOOST_LOG_TRIVIAL(info) << "Hooking USER32!DefWindowProcW (Unicode)";

	if (MH_CreateHookApiEx(L"user32", "DefWindowProcW", &DetourDefWindowProc, &OriginalDefWindowProc) != MH_OK)
	{
		BOOST_LOG_TRIVIAL(error) << "Couldn't hook USER32!DefWindowProcW (Unicode)";
		return;
	}

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
	{
		BOOST_LOG_TRIVIAL(error) << "Couldn't enable DefWindowProc hooks";
		return;
	}

	BOOST_LOG_TRIVIAL(info) << "DefWindowProc hooked";
}

LRESULT WINAPI DetourDefWindowProc(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	)
{
	static boost::once_flag flag = BOOST_ONCE_INIT;
	boost::call_once(flag, boost::bind(&logOnce, "++ USER32!DefWindowProc called"));

	if (!g_hWnd)
	{
		g_hWnd = hWnd;
	}

	ImGui_ImplDX9_WndProcHandler(hWnd, Msg, wParam, lParam);
	ImGui_ImplDX10_WndProcHandler(hWnd, Msg, wParam, lParam);
	ImGui_ImplDX11_WndProcHandler(hWnd, Msg, wParam, lParam);

	return OriginalDefWindowProc(hWnd, Msg, wParam, lParam);
}

void HookDX9(UINTX* vtable9)
{
	BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9::Present";

	g_present9Hook.apply(vtable9[Direct3D9Hooking::Present], [](LPDIRECT3DDEVICE9 dev, CONST RECT* a1, CONST RECT* a2, HWND a3, CONST RGNDATA* a4) -> HRESULT
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;
		boost::call_once(flag, boost::bind(&logOnce, "++ IDirect3DDevice9::Present called"));

		g_bIsUsingPresent = true;

		if (!g_bIsImGuiInitialized)
		{
			if (g_hWnd)
			{
				ImGui_ImplDX9_Init(g_hWnd, dev);

				BOOST_LOG_TRIVIAL(info) << "ImGui (DX9) initialized";

				g_bIsImGuiInitialized = true;
			}
		}
		else
		{
			ImGui_ImplDX9_NewFrame();
			RenderScene();
		}

		return g_present9Hook.callOrig(dev, a1, a2, a3, a4);
	});

	BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9::Reset";

	g_reset9Hook.apply(vtable9[Direct3D9Hooking::Reset], [](LPDIRECT3DDEVICE9 dev, D3DPRESENT_PARAMETERS* pp) -> HRESULT
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;
		boost::call_once(flag, boost::bind(&logOnce, "++ IDirect3DDevice9::Reset called"));

		// g_pRenderer.reset(dev);

		return g_reset9Hook.callOrig(dev, pp);
	});

	BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9::EndScene";

	g_endScene9Hook.apply(vtable9[Direct3D9Hooking::EndScene], [](LPDIRECT3DDEVICE9 dev) -> HRESULT
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;
		boost::call_once(flag, boost::bind(&logOnce, "++ IDirect3DDevice9::EndScene called"));

		if (!g_bIsUsingPresent)
		{
			if (!g_bIsImGuiInitialized)
			{
				if (g_hWnd)
				{
					ImGui_ImplDX9_Init(g_hWnd, dev);

					g_bIsImGuiInitialized = true;
				}
			}
			else
			{
				ImGui_ImplDX9_NewFrame();
				RenderScene();
			}
		}

		return g_endScene9Hook.callOrig(dev);
	});
}

void HookDX9Ex(UINTX* vtable9Ex)
{
	BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9Ex::PresentEx";

	g_present9ExHook.apply(vtable9Ex[Direct3D9Hooking::PresentEx], [](LPDIRECT3DDEVICE9EX dev, CONST RECT* a1, CONST RECT* a2, HWND a3, CONST RGNDATA* a4, DWORD a5) -> HRESULT
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;
		boost::call_once(flag, boost::bind(&logOnce, "++ IDirect3DDevice9Ex::PresentEx called"));

		g_bIsUsingPresent = true;

		if (!g_bIsImGuiInitialized)
		{
			if (g_hWnd)
			{
				ImGui_ImplDX9_Init(g_hWnd, dev);

				BOOST_LOG_TRIVIAL(info) << "ImGui (DX9Ex) initialized";

				g_bIsImGuiInitialized = true;
			}
		}
		else
		{
			ImGui_ImplDX9_NewFrame();
			RenderScene();
		}

		return g_present9ExHook.callOrig(dev, a1, a2, a3, a4, a5);
	});

	BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9Ex::ResetEx";

	g_reset9ExHook.apply(vtable9Ex[Direct3D9Hooking::ResetEx], [](LPDIRECT3DDEVICE9EX dev, D3DPRESENT_PARAMETERS* pp, D3DDISPLAYMODEEX* ppp) -> HRESULT
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;
		boost::call_once(flag, boost::bind(&logOnce, "++ IDirect3DDevice9Ex::ResetEx called"));

		// g_pRenderer.reset(dev);

		return g_reset9ExHook.callOrig(dev, pp, ppp);
	});
}

void HookDX10(UINTX* vtable10SwapChain)
{
	BOOST_LOG_TRIVIAL(info) << "Hooking IDXGISwapChain::Present";

	g_swapChainPresent10Hook.apply(vtable10SwapChain[DXGIHooking::Present], [](IDXGISwapChain* chain, UINT SyncInterval, UINT Flags) -> HRESULT
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;
		boost::call_once(flag, boost::bind(&logOnce, "++ IDXGISwapChain::Present (v10) called"));

		g_bIsUsingPresent = true;
		static auto failed = false;

		if (!failed)
		{
			if (!g_bIsImGuiInitialized)
			{
				if (g_hWnd)
				{
					static ID3D10Device* dev = nullptr;
					auto hr = chain->GetDevice(__uuidof(dev), reinterpret_cast<void**>(&dev));

					if (SUCCEEDED(hr))
					{
						ImGui_ImplDX10_Init(g_hWnd, dev);

						BOOST_LOG_TRIVIAL(info) << "ImGui (DX10) initialized";

						g_bIsImGuiInitialized = true;
					}
					else
					{
						BOOST_LOG_TRIVIAL(error) << "!! Couldn't get ID3D10Device";

						failed = true;
					}
				}
			}
			else
			{
				ImGui_ImplDX10_NewFrame();
				RenderScene();
			}
		}

		return g_swapChainPresent10Hook.callOrig(chain, SyncInterval, Flags);
	});

	BOOST_LOG_TRIVIAL(info) << "Hooking IDXGISwapChain::ResizeTarget";

	g_swapChainResizeTarget10Hook.apply(vtable10SwapChain[DXGIHooking::ResizeTarget], [](IDXGISwapChain* chain, const DXGI_MODE_DESC* pNewTargetParameters) -> HRESULT
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;
		boost::call_once(flag, boost::bind(&logOnce, "++ IDXGISwapChain::ResizeTarget (v10) called"));

		return g_swapChainResizeTarget10Hook.callOrig(chain, pNewTargetParameters);
	});
}

void HookDX11(UINTX* vtable11SwapChain)
{
	BOOST_LOG_TRIVIAL(info) << "Hooking IDXGISwapChain::Present";

	g_swapChainPresent11Hook.apply(vtable11SwapChain[DXGIHooking::Present], [](IDXGISwapChain* chain, UINT SyncInterval, UINT Flags) -> HRESULT
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;
		boost::call_once(flag, boost::bind(&logOnce, "++ IDXGISwapChain::Present (v11) called"));

		g_bIsUsingPresent = true;

		static ID3D11DeviceContext* ctx = nullptr;
		static ID3D11RenderTargetView* view = nullptr;

		if (!g_bIsImGuiInitialized)
		{
			if (g_hWnd)
			{
				static ID3D11Device* dev = nullptr;
				chain->GetDevice(__uuidof(dev), reinterpret_cast<void**>(&dev));

				dev->GetImmediateContext(&ctx);

				DXGI_SWAP_CHAIN_DESC sd;
				chain->GetDesc(&sd);

				// Create the render target
				ID3D11Texture2D* pBackBuffer;
				D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;
				ZeroMemory(&render_target_view_desc, sizeof(render_target_view_desc));
				render_target_view_desc.Format = sd.BufferDesc.Format;
				render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
				dev->CreateRenderTargetView(pBackBuffer, &render_target_view_desc, &view);
				ctx->OMSetRenderTargets(1, &view, nullptr);
				pBackBuffer->Release();

				ImGui_ImplDX11_Init(g_hWnd, dev, ctx);

				BOOST_LOG_TRIVIAL(info) << "ImGui (DX11) initialized";

				g_bIsImGuiInitialized = true;
			}
		}
		else
		{
			ImGui_ImplDX11_NewFrame();

			//static ImVec4 clear_col = ImColor(114, 144, 154, 20);
			//ctx->ClearRenderTargetView(view, (float*)&clear_col);

			RenderScene();
		}

		return g_swapChainPresent11Hook.callOrig(chain, SyncInterval, Flags);
	});

	BOOST_LOG_TRIVIAL(info) << "Hooking IDXGISwapChain::ResizeTarget";

	g_swapChainResizeTarget11Hook.apply(vtable11SwapChain[DXGIHooking::ResizeTarget], [](IDXGISwapChain* chain, const DXGI_MODE_DESC* pNewTargetParameters) -> HRESULT
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;
		boost::call_once(flag, boost::bind(&logOnce, "++ IDXGISwapChain::ResizeTarget (v11) called"));

		return g_swapChainResizeTarget11Hook.callOrig(chain, pNewTargetParameters);
	});
}

void HookDInput8(UINTX* vtable8)
{
	BOOST_LOG_TRIVIAL(info) << "Hooking IDirectInputDevice8::Acquire";

	g_acquire8Hook.apply(vtable8[DirectInput8Hooking::Acquire], [](LPDIRECTINPUTDEVICE8 dev) -> HRESULT
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;
		boost::call_once(flag, boost::bind(&logOnce, "++ IDirectInputDevice8::Acquire called"));

		return g_acquire8Hook.callOrig(dev);
	});

	BOOST_LOG_TRIVIAL(info) << "Hooking IDirectInputDevice8::GetDeviceData";

	g_getDeviceData8Hook.apply(vtable8[DirectInput8Hooking::GetDeviceData], [](LPDIRECTINPUTDEVICE8 dev, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags) -> HRESULT
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;
		boost::call_once(flag, boost::bind(&logOnce, "++ IDirectInputDevice8::Acquire called"));

		return g_getDeviceData8Hook.callOrig(dev, cbObjectData, rgdod, pdwInOut, dwFlags);
	});

	BOOST_LOG_TRIVIAL(info) << "Hooking IDirectInputDevice8::GetDeviceInfo ";

	g_getDeviceInfo8Hook.apply(vtable8[DirectInput8Hooking::GetDeviceInfo], [](LPDIRECTINPUTDEVICE8 dev, LPDIDEVICEINSTANCE pdidi) -> HRESULT
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;
		boost::call_once(flag, boost::bind(&logOnce, "++ IDirectInputDevice8::GetDeviceInfo called"));

		return g_getDeviceInfo8Hook.callOrig(dev, pdidi);
	});

	BOOST_LOG_TRIVIAL(info) << "Hooking IDirectInputDevice8::GetDeviceState";

	g_getDeviceState8Hook.apply(vtable8[DirectInput8Hooking::GetDeviceState], [](LPDIRECTINPUTDEVICE8 dev, DWORD cbData, LPVOID lpvData) -> HRESULT
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;
		boost::call_once(flag, boost::bind(&logOnce, "++ IDirectInputDevice8::GetDeviceState called"));

		return g_getDeviceState8Hook.callOrig(dev, cbData, lpvData);
	});

	BOOST_LOG_TRIVIAL(info) << "Hooking IDirectInputDevice8::GetObjectInfo";

	g_getObjectInfo8Hook.apply(vtable8[DirectInput8Hooking::GetObjectInfo], [](LPDIRECTINPUTDEVICE8 dev, LPDIDEVICEOBJECTINSTANCE pdidoi, DWORD dwObj, DWORD dwHow) -> HRESULT
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;
		boost::call_once(flag, boost::bind(&logOnce, "++ IDirectInputDevice8::GetObjectInfo called"));

		return g_getObjectInfo8Hook.callOrig(dev, pdidoi, dwObj, dwHow);
	});
}

void RenderScene()
{
	static bool show_overlay = false;
	static bool show_test_window = true;
	static bool show_another_window = false;
	static ImVec4 clear_col = ImColor(114, 144, 154);

	// 1. Show a simple window
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
	{
		static float f = 0.0f;
		ImGui::Text("Hello, world!");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::ColorEdit3("clear color", (float*)&clear_col);
		if (ImGui::Button("Test Window")) show_test_window ^= 1;
		if (ImGui::Button("Another Window")) show_another_window ^= 1;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	// 2. Show another simple window, this time using an explicit Begin/End pair
	if (show_another_window)
	{
		ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Another Window", &show_another_window);
		ImGui::Text("Hello");
		ImGui::End();
	}

	// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if (show_test_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}

	static auto pressedPast = false, pressedNow = false;
	if (GetAsyncKeyState(VK_F11) & 0x8000)
	{
		pressedNow = true;
	}
	else
	{
		pressedPast = false;
		pressedNow = false;
	}

	if (!pressedPast && pressedNow)
	{
		show_overlay = !show_overlay;

		pressedPast = true;
	}

	if (show_overlay)
	{
		ImGui::Render();
	}
}
