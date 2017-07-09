#include "dllmain.h"

#include <d3d9.h>
#include <dxgi.h>
#include <d3d11.h>

// Boost includes
#include <boost/log/trivial.hpp>
#include <boost/thread/once.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

// Boost namespaces
namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;

// ImGui includes
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_dx10.h>
#include <imgui_impl_dx11.h>

HWND g_hWnd = nullptr;
tDefWindowProc OriginalDefWindowProc = nullptr;

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	DisableThreadLibraryCalls(static_cast<HMODULE>(hInstance));

	if (dwReason != DLL_PROCESS_ATTACH)
		return TRUE;

	logging::add_common_attributes();

	logging::add_file_log
		(
			keywords::file_name = "Indicium-ImGui.Plugin.log",
			keywords::auto_flush = true,
			keywords::format = "[%TimeStamp%]: %Message%"
		);

	logging::core::get()->set_filter
		(
			logging::trivial::severity >= logging::trivial::info
		);

	return CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(init), nullptr, 0, nullptr) > nullptr;
}

int init()
{
	BOOST_LOG_TRIVIAL(info) << "Initializing hook engine...";

	if (MH_Initialize() != MH_OK)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Couldn't initialize hook engine";
		return -1;
	}

	BOOST_LOG_TRIVIAL(info) << "Hook engine initialized";

	HookDefWindowProc();

	return WaitForSingleObject(INVALID_HANDLE_VALUE, INFINITE);
}

void logInfo(std::string message)
{
	BOOST_LOG_TRIVIAL(info) << message;
}

INDICIUM_EXPORT Present(IID guid, LPVOID unknown)
{
	static auto bIsImGuiInitialized = false;

	if (guid == IID_IDirect3DDevice9)
	{
		if (!bIsImGuiInitialized)
		{
			if (g_hWnd)
			{
				ImGui_ImplDX9_Init(g_hWnd, static_cast<IDirect3DDevice9*>(unknown));

				BOOST_LOG_TRIVIAL(info) << "ImGui (DX9) initialized";

				bIsImGuiInitialized = true;
			}
		}
		else
		{
			ImGui_ImplDX9_NewFrame();
			RenderScene();
		}
	}
	else if (guid == IID_IDirect3DDevice9Ex)
	{
		if (!bIsImGuiInitialized)
		{
			if (g_hWnd)
			{
				ImGui_ImplDX9_Init(g_hWnd, static_cast<IDirect3DDevice9Ex*>(unknown));

				BOOST_LOG_TRIVIAL(info) << "ImGui (DX9Ex) initialized";

				bIsImGuiInitialized = true;
			}
		}
		else
		{
			ImGui_ImplDX9_NewFrame();
			RenderScene();
		}
	}
	else if (guid == IID_IDXGISwapChain)
	{
		auto chain = static_cast<IDXGISwapChain*>(unknown);

		static ID3D11DeviceContext* ctx = nullptr;
		static ID3D11RenderTargetView* view = nullptr;
		static ID3D11Device* dev = nullptr;

		if (!bIsImGuiInitialized)
		{
			// window handle available, initialize
			if (g_hWnd)
			{
				// get device
				chain->GetDevice(__uuidof(dev), reinterpret_cast<void**>(&dev));

				// get device context
				dev->GetImmediateContext(&ctx);

				// initialize ImGui
				ImGui_ImplDX11_Init(g_hWnd, dev, ctx);

				BOOST_LOG_TRIVIAL(info) << "ImGui (DX11) initialized";

				bIsImGuiInitialized = true;
			}
		}
		else
		{
			DXGI_SWAP_CHAIN_DESC sd;
			chain->GetDesc(&sd);

			if (view)
				view->Release();

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

			ImGui_ImplDX11_NewFrame();

			RenderScene();
		}
	}
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
	boost::call_once(flag, boost::bind(&logInfo, "++ USER32!DefWindowProc called"));

	if (!g_hWnd)
	{
		g_hWnd = hWnd;
	}

	ImGui_ImplDX9_WndProcHandler(hWnd, Msg, wParam, lParam);
	ImGui_ImplDX10_WndProcHandler(hWnd, Msg, wParam, lParam);
	ImGui_ImplDX11_WndProcHandler(hWnd, Msg, wParam, lParam);

	return OriginalDefWindowProc(hWnd, Msg, wParam, lParam);
}

void RenderScene()
{
    static boost::once_flag flag = BOOST_ONCE_INIT;
    boost::call_once(flag, boost::bind(&logInfo, "++ RenderScene called"));

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

