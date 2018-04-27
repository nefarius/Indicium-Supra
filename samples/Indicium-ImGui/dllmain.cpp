#include "dllmain.h"

// 
// Indicium Plugin
// 
#include <Indicium/Plugin/Direct3D9.h>
#include <Indicium/Plugin/Direct3D9Ex.h>
#include <Indicium/Plugin/Direct3D10.h>
#include <Indicium/Plugin/Direct3D11.h>

// 
// MinHook
// 
#include <MinHook.h>

// 
// STL
// 
#include <mutex>
#include <map>

// 
// POCO
// 
#include <Poco/Message.h>
#include <Poco/Logger.h>
#include <Poco/FileChannel.h>
#include <Poco/AutoPtr.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/Path.h>

using Poco::Message;
using Poco::Logger;
using Poco::FileChannel;
using Poco::AutoPtr;
using Poco::PatternFormatter;
using Poco::FormattingChannel;
using Poco::Path;

// 
// ImGui includes
// 
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_dx10.h>
#include <imgui_impl_dx11.h>

t_WindowProc OriginalWindowProc = nullptr;


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
    DisableThreadLibraryCalls(static_cast<HMODULE>(hInstance));

    return TRUE;
}

INDICIUM_EXPORT(BOOLEAN) indicium_plugin_init(Direct3DVersion version)
{
    std::string logfile("%TEMP%\\Indicium-ImGui.Plugin.log");

    AutoPtr<FileChannel> pFileChannel(new FileChannel);
    pFileChannel->setProperty("path", Poco::Path::expand(logfile));
    AutoPtr<PatternFormatter> pPF(new PatternFormatter);
    pPF->setProperty("pattern", "%Y-%m-%d %H:%M:%S.%i %s [%p]: %t");
    AutoPtr<FormattingChannel> pFC(new FormattingChannel(pPF, pFileChannel));

    Logger::root().setChannel(pFC);

    auto& logger = Logger::get(__func__);

    logger.information("Loading ImGui plugin");

    logger.information("Initializing hook engine...");

    if (MH_Initialize() != MH_OK)
    {
        logger.fatal("Couldn't initialize hook engine");
        return FALSE;
    }

    logger.information("Hook engine initialized");

    //
    // We support all version of Direct3D so we don't bother checking
    // 
    return TRUE;
}

INDICIUM_EXPORT(VOID) indicium_plugin_d3d9_present(
    LPDIRECT3DDEVICE9   pDevice,
    const RECT          *pSourceRect,
    const RECT          *pDestRect,
    HWND                hDestWindowOverride,
    const RGNDATA       *pDirtyRegion
)
{
    static auto& logger = Logger::get(__func__);
    static auto initialized = false;
    static std::once_flag init;

    std::call_once(init, [&](LPDIRECT3DDEVICE9 pd3dDevice)
    {
        D3DDEVICE_CREATION_PARAMETERS params;

        auto hr = pd3dDevice->GetCreationParameters(&params);
        if (FAILED(hr))
        {
            logger.error("Couldn't get creation parameters from device");
            return;
        }

        ImGui_ImplDX9_Init(params.hFocusWindow, pd3dDevice);

        logger.information("ImGui (DX9) initialized");

        HookWindowProc(params.hFocusWindow);

        initialized = true;

    }, pDevice);

    if (initialized)
    {
        ImGui_ImplDX9_NewFrame();
        RenderScene();
    }
}

INDICIUM_EXPORT(VOID) indicium_plugin_d3d9_reset(
    LPDIRECT3DDEVICE9       pDevice,
    D3DPRESENT_PARAMETERS   *pPresentationParameters
)
{

}

INDICIUM_EXPORT(VOID) indicium_plugin_d3d9_endscene(
    LPDIRECT3DDEVICE9 pDevice
)
{

}

INDICIUM_EXPORT(VOID) indicium_plugin_d3d9_presentex(
    LPDIRECT3DDEVICE9EX     pDevice,
    const RECT              *pSourceRect,
    const RECT              *pDestRect,
    HWND                    hDestWindowOverride,
    const RGNDATA           *pDirtyRegion,
    DWORD                   dwFlags
)
{
    static auto& logger = Logger::get(__func__);
    static auto initialized = false;
    static std::once_flag init;

    std::call_once(init, [&](LPDIRECT3DDEVICE9EX pd3dDevice)
    {
        D3DDEVICE_CREATION_PARAMETERS params;

        auto hr = pd3dDevice->GetCreationParameters(&params);
        if (FAILED(hr))
        {
            logger.error("Couldn't get creation parameters from device");
            return;
        }

        ImGui_ImplDX9_Init(params.hFocusWindow, pd3dDevice);

        logger.information("ImGui (DX9Ex) initialized");

        HookWindowProc(params.hFocusWindow);

        initialized = true;

    }, pDevice);

    if (initialized)
    {
        ImGui_ImplDX9_NewFrame();
        RenderScene();
    }
}

INDICIUM_EXPORT(VOID) indicium_plugin_d3d9_resetex(
    LPDIRECT3DDEVICE9EX     pDevice,
    D3DPRESENT_PARAMETERS   *pPresentationParameters,
    D3DDISPLAYMODEEX        *pFullscreenDisplayMode
)
{

}

INDICIUM_EXPORT(VOID) indicium_plugin_d3d10_present(
    IDXGISwapChain  *pSwapChain,
    UINT            SyncInterval,
    UINT            Flags
)
{
    static auto& logger = Logger::get(__func__);
    static auto initialized = false;
    static std::once_flag init;

    std::call_once(init, [&](IDXGISwapChain *pChain)
    {
        logger.information("Grabbing device and context pointers");

        ID3D10Device *pDevice;
        if (FAILED(D3D10_DEVICE_FROM_SWAPCHAIN(pChain, &pDevice)))
        {
            logger.error("Couldn't get device from swapchain");
            return;
        }

        DXGI_SWAP_CHAIN_DESC sd;
        pChain->GetDesc(&sd);

        logger.information("Initializing ImGui");

        ImGui_ImplDX10_Init(sd.OutputWindow, pDevice);

        logger.information("ImGui (DX10) initialized");

        HookWindowProc(sd.OutputWindow);

        initialized = true;

    }, pSwapChain);

    if (initialized)
    {
        ImGui_ImplDX10_NewFrame();
        RenderScene();
    }
}

INDICIUM_EXPORT(VOID) indicium_plugin_d3d10_resizetarget(
    IDXGISwapChain          *pSwapChain,
    const DXGI_MODE_DESC    *pNewTargetParameters
)
{

}

INDICIUM_EXPORT(VOID) indicium_plugin_d3d11_present(
    IDXGISwapChain  *pSwapChain,
    UINT            SyncInterval,
    UINT            Flags
)
{
    static auto& logger = Logger::get(__func__);
    static auto initialized = false;
    static std::once_flag init;

    static ID3D11DeviceContext *pContext;
    static ID3D11RenderTargetView *mainRenderTargetView;

    std::call_once(init, [&](IDXGISwapChain *pChain)
    {
        logger.information("Grabbing device and context pointers");

        ID3D11Device *pDevice;
        if (FAILED(D3D11_DEVICE_CONTEXT_FROM_SWAPCHAIN(pChain, &pDevice, &pContext)))
        {
            logger.error("Couldn't get device and context from swapchain");
            return;
        }

        ID3D11Texture2D* pBackBuffer;
        pChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
        pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
        pBackBuffer->Release();

        DXGI_SWAP_CHAIN_DESC sd;
        pChain->GetDesc(&sd);

        logger.information("Initializing ImGui");

        ImGui_ImplDX11_Init(sd.OutputWindow, pDevice, pContext);

        logger.information("ImGui (DX11) initialized");

        HookWindowProc(sd.OutputWindow);

        initialized = true;

    }, pSwapChain);

    if (initialized)
    {
        ImGui_ImplDX11_NewFrame();
        pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
        RenderScene();
    }
}

INDICIUM_EXPORT(VOID) indicium_plugin_d3d11_resizetarget(
    IDXGISwapChain          *pSwapChain,
    const DXGI_MODE_DESC    *pNewTargetParameters
)
{

}

void HookWindowProc(HWND hWnd)
{
    auto& logger = Logger::get(__func__);

    auto lptrWndProc = reinterpret_cast<t_WindowProc>(GetWindowLongPtr(hWnd, GWLP_WNDPROC));

    if (MH_CreateHook(lptrWndProc, &DetourWindowProc, reinterpret_cast<LPVOID*>(&OriginalWindowProc)) != MH_OK)
    {
        logger.error("Coudln't create hook for WNDPROC");
        return;
    }

    if (MH_EnableHook(lptrWndProc) != MH_OK)
    {
        logger.error("Couldn't enable DefWindowProc hooks");
        return;
    }

    logger.information("WindowProc hooked");
}

LRESULT WINAPI DetourWindowProc(
    _In_ HWND hWnd,
    _In_ UINT Msg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
)
{
    static std::once_flag flag;
    std::call_once(flag, []() {Logger::get("DetourWindowProc").information("++ DetourWindowProc called"); });

    //
    // TODO: this is actually wrong and stupid, fix!
    // 
    ImGui_ImplDX9_WndProcHandler(hWnd, Msg, wParam, lParam);
    ImGui_ImplDX10_WndProcHandler(hWnd, Msg, wParam, lParam);
    ImGui_ImplDX11_WndProcHandler(hWnd, Msg, wParam, lParam);

    return OriginalWindowProc(hWnd, Msg, wParam, lParam);
}

void RenderScene()
{
    static std::once_flag flag;
    std::call_once(flag, []() {Logger::get("RenderScene").information("++ RenderScene called"); });

    static bool show_overlay = true;
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
