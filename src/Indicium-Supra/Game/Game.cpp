#include <Utils/Windows.h>
#include <Utils/Hook.h>

#include <MinHook.h>

#include "Game.h"

#include <Psapi.h>

#include <Game/Hook/Direct3D9.h>
#include <Game/Hook/Direct3D9Ex.h>
#include <Game/Hook/DXGI.h>
#include <Game/Hook/Direct3D10.h>
#include <Game/Hook/Direct3D11.h>
#include <Game/Hook/Direct3D12.h>
#include <Game/Hook/DirectInput8.h>

#include <Utils/PluginManager.h>

#include <mutex>

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/AutoPtr.h>

using Poco::Logger;
using Poco::AutoPtr;


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

// D3D12
Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, UINT, UINT> g_swapChainPresent12Hook;
Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, const DXGI_MODE_DESC*> g_swapChainResizeTarget12Hook;


// DInput8
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECTINPUTDEVICE8> g_acquire8Hook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECTINPUTDEVICE8, DWORD, LPDIDEVICEOBJECTDATA, LPDWORD, DWORD> g_getDeviceData8Hook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECTINPUTDEVICE8, LPDIDEVICEINSTANCE> g_getDeviceInfo8Hook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECTINPUTDEVICE8, DWORD, LPVOID> g_getDeviceState8Hook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECTINPUTDEVICE8, LPDIDEVICEOBJECTINSTANCE, DWORD, DWORD> g_getObjectInfo8Hook;


PluginManager g_plugins;


void initGame()
{
    auto& logger = Logger::get(__func__);

    bool dinput8_available;

    auto sz_ProcName = static_cast<LPSTR>(malloc(MAX_PATH + 1));
    GetProcessImageFileName(GetCurrentProcess(), sz_ProcName, MAX_PATH);
    logger.information("Library loaded into %s", std::string(sz_ProcName));
    free(sz_ProcName);

    logger.information("Library enabled");

    g_plugins.load();

    UINTX vtable8[DirectInput8Hooking::DirectInput8::VTableElements] = { 0 };

    logger.information("Initializing hook engine...");

    if (MH_Initialize() != MH_OK)
    {
        logger.fatal("Couldn't initialize hook engine");
        return;
    }

    logger.information("Hook engine initialized");

#pragma region D3D9

    try
    {
        AutoPtr<Direct3D9Hooking::Direct3D9> d3d(new Direct3D9Hooking::Direct3D9);
        auto vtable = d3d->vtable();

        logger.information("Hooking IDirect3DDevice9::Present");

        g_present9Hook.apply(vtable[Direct3D9Hooking::Present], [](LPDIRECT3DDEVICE9 dev, CONST RECT* a1, CONST RECT* a2, HWND a3, CONST RGNDATA* a4) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []() { Logger::get("HookDX9").information("++ IDirect3DDevice9::Present called"); });

            g_plugins.present(IID_IDirect3DDevice9, dev, Direct3DVersion::Direct3D9);

            return g_present9Hook.callOrig(dev, a1, a2, a3, a4);
        });

        logger.information("Hooking IDirect3DDevice9::Reset");

        g_reset9Hook.apply(vtable[Direct3D9Hooking::Reset], [](LPDIRECT3DDEVICE9 dev, D3DPRESENT_PARAMETERS* pp) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []() { Logger::get("HookDX9").information("++ IDirect3DDevice9::Reset called"); });

            g_plugins.reset(IID_IDirect3DDevice9, dev, Direct3DVersion::Direct3D9);

            return g_reset9Hook.callOrig(dev, pp);
        });

        logger.information("Hooking IDirect3DDevice9::EndScene");

        g_endScene9Hook.apply(vtable[Direct3D9Hooking::EndScene], [](LPDIRECT3DDEVICE9 dev) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []() { Logger::get("HookDX9").information("++ IDirect3DDevice9::EndScene called"); });

            g_plugins.endScene(IID_IDirect3DDevice9, dev, Direct3DVersion::Direct3D9);

            return g_endScene9Hook.callOrig(dev);
        });
    }
    catch (Poco::Exception& pex)
    {
        logger.error(pex.displayText());
    }

#pragma endregion

#pragma region D3D9Ex

    try
    {
        // get VTable for Direct3DCreate9Ex
        AutoPtr<Direct3D9Hooking::Direct3D9Ex> d3dEx(new Direct3D9Hooking::Direct3D9Ex);
        auto vtable = d3dEx->vtable();

        logger.information("Hooking IDirect3DDevice9Ex::PresentEx");

        g_present9ExHook.apply(vtable[Direct3D9Hooking::PresentEx], [](LPDIRECT3DDEVICE9EX dev, CONST RECT* a1, CONST RECT* a2, HWND a3, CONST RGNDATA* a4, DWORD a5) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []() { Logger::get("HookDX9Ex").information("++ IDirect3DDevice9Ex::PresentEx called"); });

            g_plugins.present(IID_IDirect3DDevice9Ex, dev, Direct3DVersion::Direct3D9Ex);

            return g_present9ExHook.callOrig(dev, a1, a2, a3, a4, a5);
        });

        logger.information("Hooking IDirect3DDevice9Ex::ResetEx");

        g_reset9ExHook.apply(vtable[Direct3D9Hooking::ResetEx], [](LPDIRECT3DDEVICE9EX dev, D3DPRESENT_PARAMETERS* pp, D3DDISPLAYMODEEX* ppp) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []() { Logger::get("HookDX9Ex").information("++ IDirect3DDevice9Ex::ResetEx called"); });

            g_plugins.reset(IID_IDirect3DDevice9Ex, dev, Direct3DVersion::Direct3D9Ex);

            return g_reset9ExHook.callOrig(dev, pp, ppp);
        });
    }
    catch (Poco::Exception& pex)
    {
        logger.error(pex.displayText());
    }

#pragma endregion

#pragma region D3D10

    try
    {
        // get VTable for IDXGISwapChain (v10)
        AutoPtr<Direct3D10Hooking::Direct3D10> d3d10(new Direct3D10Hooking::Direct3D10);
        auto vtable = d3d10->vtable();

        logger.information("Hooking IDXGISwapChain::Present");

        g_swapChainPresent10Hook.apply(vtable[DXGIHooking::Present], [](IDXGISwapChain* chain, UINT SyncInterval, UINT Flags) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []() { Logger::get("HookDX10").information("++ IDXGISwapChain::Present called"); });

            g_plugins.present(IID_IDXGISwapChain, chain, Direct3DVersion::Direct3D10);

            return g_swapChainPresent10Hook.callOrig(chain, SyncInterval, Flags);
        });

        logger.information("Hooking IDXGISwapChain::ResizeTarget");

        g_swapChainResizeTarget10Hook.apply(vtable[DXGIHooking::ResizeTarget], [](IDXGISwapChain* chain, const DXGI_MODE_DESC* pNewTargetParameters) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []() { Logger::get("HookDX10").information("++ IDXGISwapChain::ResizeTarget called"); });

            g_plugins.resizeTarget(IID_IDXGISwapChain, chain, Direct3DVersion::Direct3D10);

            return g_swapChainResizeTarget10Hook.callOrig(chain, pNewTargetParameters);
        });
    }
    catch (Poco::Exception& pex)
    {
        logger.error(pex.displayText());
    }

#pragma endregion

#pragma region D3D11

    try
    {
        // get VTable for IDXGISwapChain (v11)
        AutoPtr<Direct3D11Hooking::Direct3D11> d3d11(new Direct3D11Hooking::Direct3D11);
        auto vtable = d3d11->vtable();

        logger.information("Hooking IDXGISwapChain::Present");

        g_swapChainPresent11Hook.apply(vtable[DXGIHooking::Present], [](IDXGISwapChain* chain, UINT SyncInterval, UINT Flags) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []() { Logger::get("HookDX11").information("++ IDXGISwapChain::Present called"); });

            g_plugins.present(IID_IDXGISwapChain, chain, Direct3DVersion::Direct3D11);

            return g_swapChainPresent11Hook.callOrig(chain, SyncInterval, Flags);
        });

        logger.information("Hooking IDXGISwapChain::ResizeTarget");

        g_swapChainResizeTarget11Hook.apply(vtable[DXGIHooking::ResizeTarget], [](IDXGISwapChain* chain, const DXGI_MODE_DESC* pNewTargetParameters) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []() { Logger::get("HookDX11").information("++ IDXGISwapChain::ResizeTarget called"); });

            g_plugins.resizeTarget(IID_IDXGISwapChain, chain, Direct3DVersion::Direct3D11);

            return g_swapChainResizeTarget11Hook.callOrig(chain, pNewTargetParameters);
        });
    }
    catch (Poco::Exception& pex)
    {
        logger.error(pex.displayText());
    }

#pragma endregion

#pragma region D3D12

    try {
        // get VTable for IDXGISwapChain (v12)
        AutoPtr<Direct3D12Hooking::Direct3D12> d3d12(new Direct3D12Hooking::Direct3D12);
        auto vtable = d3d12->vtable();

        logger.information("Hooking IDXGISwapChain::Present");

        g_swapChainPresent12Hook.apply(vtable[DXGIHooking::Present], [](IDXGISwapChain* chain, UINT SyncInterval, UINT Flags) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []() { Logger::get("HookDX12").information("++ IDXGISwapChain::Present called"); });

            g_plugins.present(IID_IDXGISwapChain, chain, Direct3DVersion::Direct3D12);

            return g_swapChainPresent12Hook.callOrig(chain, SyncInterval, Flags);
        });

        logger.information("Hooking IDXGISwapChain::ResizeTarget");

        g_swapChainResizeTarget12Hook.apply(vtable[DXGIHooking::ResizeTarget], [](IDXGISwapChain* chain, const DXGI_MODE_DESC* pNewTargetParameters) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []() { Logger::get("HookDX12").information("++ IDXGISwapChain::ResizeTarget called"); });

            g_plugins.resizeTarget(IID_IDXGISwapChain, chain, Direct3DVersion::Direct3D12);

            return g_swapChainResizeTarget12Hook.callOrig(chain, pNewTargetParameters);
        });
    }
    catch (Poco::Exception& pex)
    {
        logger.error(pex.displayText());
    }

#pragma endregion

    // Dinput8
    {
        DirectInput8Hooking::DirectInput8 di8;
        dinput8_available = di8.GetVTable(vtable8);

        if (!dinput8_available)
        {
            logger.warning("Couldn't get VTable for DirectInput8");
        }
    }

    if (dinput8_available)
    {
        logger.information("Game uses DirectInput8");
        HookDInput8(vtable8);
    }

    logger.information("Library initialized successfully");

    /*
     * TODO: implement proper shutdown and clean-up
    logger.information("Shutting down hooks...");

    if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK)
    {
        logger.fatal("Couldn't disable hooks, host process might crash");
    }
    else
    {
        logger.information("Hooks disabled");
    }

    if (MH_Uninitialize() != MH_OK)
    {
        logger.fatal("Couldn't shut down hook engine, host process might crash");
    }
    else
    {
        logger.information("Hook engine disabled");
    }
    */
}

void HookDInput8(UINTX* vtable8)
{
    auto& logger = Logger::get(__func__);
    logger.information("Hooking IDirectInputDevice8::Acquire");

    g_acquire8Hook.apply(vtable8[DirectInput8Hooking::Acquire], [](LPDIRECTINPUTDEVICE8 dev) -> HRESULT
    {
        static std::once_flag flag;
        std::call_once(flag, []() { Logger::get("HookDInput8").information("++ IDirectInputDevice8::Acquire called"); });

        return g_acquire8Hook.callOrig(dev);
    });

    logger.information("Hooking IDirectInputDevice8::GetDeviceData");

    g_getDeviceData8Hook.apply(vtable8[DirectInput8Hooking::GetDeviceData], [](LPDIRECTINPUTDEVICE8 dev, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags) -> HRESULT
    {
        static std::once_flag flag;
        std::call_once(flag, []() { Logger::get("HookDInput8").information("++ IDirectInputDevice8::Acquire called"); });

        return g_getDeviceData8Hook.callOrig(dev, cbObjectData, rgdod, pdwInOut, dwFlags);
    });

    logger.information("Hooking IDirectInputDevice8::GetDeviceInfo");

    g_getDeviceInfo8Hook.apply(vtable8[DirectInput8Hooking::GetDeviceInfo], [](LPDIRECTINPUTDEVICE8 dev, LPDIDEVICEINSTANCE pdidi) -> HRESULT
    {
        static std::once_flag flag;
        std::call_once(flag, []() { Logger::get("HookDInput8").information("++ IDirectInputDevice8::GetDeviceInfo called"); });

        return g_getDeviceInfo8Hook.callOrig(dev, pdidi);
    });

    logger.information("Hooking IDirectInputDevice8::GetDeviceState");

    g_getDeviceState8Hook.apply(vtable8[DirectInput8Hooking::GetDeviceState], [](LPDIRECTINPUTDEVICE8 dev, DWORD cbData, LPVOID lpvData) -> HRESULT
    {
        static std::once_flag flag;
        std::call_once(flag, []() { Logger::get("HookDInput8").information("++ IDirectInputDevice8::GetDeviceState called"); });

        return g_getDeviceState8Hook.callOrig(dev, cbData, lpvData);
    });

    logger.information("Hooking IDirectInputDevice8::GetObjectInfo");

    g_getObjectInfo8Hook.apply(vtable8[DirectInput8Hooking::GetObjectInfo], [](LPDIRECTINPUTDEVICE8 dev, LPDIDEVICEOBJECTINSTANCE pdidoi, DWORD dwObj, DWORD dwHow) -> HRESULT
    {
        static std::once_flag flag;
        std::call_once(flag, []() { Logger::get("HookDInput8").information("++ IDirectInputDevice8::GetObjectInfo called"); });

        return g_getObjectInfo8Hook.callOrig(dev, pdidoi, dwObj, dwHow);
    });
}

