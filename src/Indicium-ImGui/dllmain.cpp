#include "dllmain.h"
#include "../../include/IndiciumPlugin.h"

#include <d3d9.h>
#include <dxgi.h>
#include <d3d11.h>

// POCO
#include <Poco/Message.h>
#include <Poco/Logger.h>
#include <Poco/FileChannel.h>
#include <Poco/AutoPtr.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <mutex>

using Poco::Message;
using Poco::Logger;
using Poco::FileChannel;
using Poco::AutoPtr;
using Poco::PatternFormatter;
using Poco::FormattingChannel;

// ImGui includes
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_dx10.h>
#include <imgui_impl_dx11.h>

tDefWindowProc OriginalDefWindowProc = nullptr;

static ID3D10Device*            g_pd3d10Device = nullptr;
static ID3D11Device*            g_pd3d11Device = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;

static std::once_flag d3d9Init;
static std::once_flag d3d9exInit;
static std::once_flag d3d10Init;
static std::once_flag d3d11Init;

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
    DisableThreadLibraryCalls(static_cast<HMODULE>(hInstance));

    if (dwReason != DLL_PROCESS_ATTACH)
        return TRUE;

    AutoPtr<FileChannel> pFileChannel(new FileChannel);
    pFileChannel->setProperty("path", "Indicium-ImGui.Plugin.log");
    AutoPtr<PatternFormatter> pPF(new PatternFormatter);
    pPF->setProperty("pattern", "%Y-%m-%d %H:%M:%S.%i %s [%p]: %t");
    AutoPtr<FormattingChannel> pFC(new FormattingChannel(pPF, pFileChannel));

    Logger::root().setChannel(pFC);

    auto& logger = Logger::get("DLL_PROCESS_ATTACH");

    logger.information("Loading ImGui plugin");

    return CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(init), nullptr, 0, nullptr) > nullptr;
}

int init()
{
    auto& logger = Logger::get("init");

    logger.information("Initializing hook engine...");

    if (MH_Initialize() != MH_OK)
    {
        logger.fatal("Couldn't initialize hook engine");
        return -1;
    }

    logger.information("Hook engine initialized");

    HookDefWindowProc();

    return 0;
}

INDICIUM_EXPORT Present(IID guid, LPVOID unknown)
{
    static auto& logger = Logger::get("Present");
    static auto bIsImGuiInitialized = false;

    if (guid == IID_IDirect3DDevice9)
    {
        std::call_once(d3d9Init, [&](LPVOID pUnknown)
        {
            auto pd3dDevice = static_cast<IDirect3DDevice9*>(unknown);

            D3DDEVICE_CREATION_PARAMETERS params;

            auto hr = pd3dDevice->GetCreationParameters(&params);
            if (FAILED(hr))
            {
                logger.error("Couldn't get device from swapchain");
                return;
            }

            ImGui_ImplDX9_Init(params.hFocusWindow, pd3dDevice);

            logger.information("ImGui (DX9) initialized");

        }, unknown);

        ImGui_ImplDX9_NewFrame();
        RenderScene();
    }
    else if (guid == IID_IDirect3DDevice9Ex)
    {
        std::call_once(d3d9Init, [&](LPVOID pUnknown)
        {
            auto pd3dDevice = static_cast<IDirect3DDevice9Ex*>(unknown);

            D3DDEVICE_CREATION_PARAMETERS params;

            auto hr = pd3dDevice->GetCreationParameters(&params);
            if (FAILED(hr))
            {
                logger.error("Couldn't get device from swapchain");
                return;
            }

            ImGui_ImplDX9_Init(params.hFocusWindow, pd3dDevice);

            logger.information("ImGui (DX9Ex) initialized");

        }, unknown);

        ImGui_ImplDX9_NewFrame();
        RenderScene();
    }
    else if (guid == IID_IDXGISwapChain)
    {
        //
        // TODO: add version detection
        // 

        std::call_once(d3d10Init, [&](LPVOID pChain)
        {
            logger.information("Grabbing device and context pointers");

            g_pSwapChain = static_cast<IDXGISwapChain*>(pChain);

            // get device
            auto hr = g_pSwapChain->GetDevice(__uuidof(g_pd3d10Device), reinterpret_cast<void**>(&g_pd3d10Device));
            if (FAILED(hr))
            {
                logger.error("Couldn't get device from swapchain");
                return;
            }

            DXGI_SWAP_CHAIN_DESC sd;
            g_pSwapChain->GetDesc(&sd);

            logger.information("Initializing ImGui");

            ImGui_ImplDX10_Init(sd.OutputWindow, g_pd3d10Device);

        }, unknown);

        ImGui_ImplDX10_NewFrame();

        /*
        std::call_once(d3d11Init, [&](LPVOID pChain)
        {
            logger.information("Grabbing device and context pointers");

            g_pSwapChain = static_cast<IDXGISwapChain*>(pChain);

            // get device
            auto hr = g_pSwapChain->GetDevice(__uuidof(g_pd3d11Device), reinterpret_cast<void**>(&g_pd3d11Device));
            if (FAILED(hr))
            {
                logger.error("Couldn't get device from swapchain");
                return;
            }

            // get device context
            g_pd3d11Device->GetImmediateContext(&g_pd3dDeviceContext);

            DXGI_SWAP_CHAIN_DESC sd;
            g_pSwapChain->GetDesc(&sd);

            logger.information("Initializing ImGui");

            ImGui_ImplDX11_Init(sd.OutputWindow, g_pd3d11Device, g_pd3dDeviceContext);

        }, unknown);

        ImGui_ImplDX11_NewFrame();
        */

        RenderScene();
    }
}

void HookDefWindowProc()
{
    auto& logger = Logger::get("HookDefWindowProc");

    logger.information("Hooking USER32!DefWindowProcA (ANSI)");

    if (MH_CreateHookApiEx(L"user32", "DefWindowProcA", &DetourDefWindowProc, &OriginalDefWindowProc) != MH_OK)
    {
        logger.error("Couldn't hook USER32!DefWindowProcA (ANSI)");
    }

    logger.information("Hooking USER32!DefWindowProcW (Unicode)");

    if (MH_CreateHookApiEx(L"user32", "DefWindowProcW", &DetourDefWindowProc, &OriginalDefWindowProc) != MH_OK)
    {
        logger.error("Couldn't hook USER32!DefWindowProcW (Unicode)");
    }

    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
    {
        logger.error("Couldn't enable DefWindowProc hooks");
        return;
    }

    logger.information("DefWindowProc hooked");
}

LRESULT WINAPI DetourDefWindowProc(
    _In_ HWND hWnd,
    _In_ UINT Msg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
)
{
    static std::once_flag flag;
    std::call_once(flag, []() {Logger::get("DetourDefWindowProc").information("++ USER32!DefWindowProc called"); });

    ImGui_ImplDX9_WndProcHandler(hWnd, Msg, wParam, lParam);
    ImGui_ImplDX10_WndProcHandler(hWnd, Msg, wParam, lParam);
    ImGui_ImplDX11_WndProcHandler(hWnd, Msg, wParam, lParam);

    return OriginalDefWindowProc(hWnd, Msg, wParam, lParam);
}

void RenderScene()
{
    static std::once_flag flag;
    std::call_once(flag, []() {Logger::get("RenderScene").information("++ RenderScene called"); });

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
