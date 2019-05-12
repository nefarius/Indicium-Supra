/*
MIT License

Copyright (c) 2018 Benjamin Höglinger

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <Utils/Hook.h>

#include "Game.h"
#include "Global.h"
#include "Exceptions.hpp"
using namespace Indicium::Core::Exceptions;

//
// Hooking helper sub-systems
// 
#include <Game/Hook/Direct3D9.h>
#include <Game/Hook/Direct3D9Ex.h>
#include <Game/Hook/DXGI.h>
#include <Game/Hook/Direct3D10.h>
#include <Game/Hook/Direct3D11.h>
#include <Game/Hook/Direct3D12.h>
#include <Game/Hook/DirectInput8.h>

//
// Public
// 
#include "Indicium/Engine/IndiciumCore.h"
#include "Indicium/Engine/IndiciumDirect3D9.h"
#include "Indicium/Engine/IndiciumDirect3D10.h"
#include "Indicium/Engine/IndiciumDirect3D11.h"
#include "Indicium/Engine/IndiciumDirect3D12.h"

//
// Internal
// 
#include "Engine.h"

//
// STL
// 
#include <mutex>

//
// Boost
//
#include <boost/log/trivial.hpp>
#include <boost/log/attributes/named_scope.hpp>

//
// POCO
// 
#include <Poco/AutoPtr.h>
#include <Poco/Buffer.h>
#include <Poco/Util/IniFileConfiguration.h>
#include <Poco/Path.h>

using Poco::AutoPtr;
using Poco::Buffer;
using Poco::Util::IniFileConfiguration;
using Poco::Path;

// NOTE: DirectInput hooking is technically implemented but not really useful
// #define HOOK_DINPUT8

#ifdef HOOK_DINPUT8
// DInput8
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECTINPUTDEVICE8> g_acquire8Hook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECTINPUTDEVICE8, DWORD, LPDIDEVICEOBJECTDATA, LPDWORD, DWORD> g_getDeviceData8Hook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECTINPUTDEVICE8, LPDIDEVICEINSTANCE> g_getDeviceInfo8Hook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECTINPUTDEVICE8, DWORD, LPVOID> g_getDeviceState8Hook;
Hook<CallConvention::stdcall_t, HRESULT, LPDIRECTINPUTDEVICE8, LPDIDEVICEOBJECTINSTANCE, DWORD, DWORD> g_getObjectInfo8Hook;

void HookDInput8(size_t* vtable8);
#endif

/**
 * \fn  void IndiciumMainThread(LPVOID Params)
 *
 * \brief   Indicium Engine main thread. All the hooking happens here.
 *
 * \author  Benjamin "Nefarius" Höglinger
 * \date    13.06.2018
 *
 * \param   Params  Options for controlling the operation.
 */
DWORD WINAPI IndiciumMainThread(LPVOID Params)
{
    BOOST_LOG_NAMED_SCOPE(__func__);

    static PINDICIUM_ENGINE engine = reinterpret_cast<PINDICIUM_ENGINE>(Params);

    BOOST_LOG_TRIVIAL(info) << "Library loaded into " << GlobalState::instance().process_name();

    BOOST_LOG_TRIVIAL(info) << "Library enabled";

    // 
    // D3D9 Hooks
    // 
    static Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *> present9Hook;
    static Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS *> reset9Hook;
    static Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9> endScene9Hook;

    // 
    // D3D9Ex Hooks
    // 
    static Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9EX, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *, DWORD> present9ExHook;
    static Hook<CallConvention::stdcall_t, HRESULT, LPDIRECT3DDEVICE9EX, D3DPRESENT_PARAMETERS *, D3DDISPLAYMODEEX *> reset9ExHook;

    // 
    // D3D10 Hooks
    // 
    static Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, UINT, UINT> swapChainPresent10Hook;
    static Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, const DXGI_MODE_DESC*> swapChainResizeTarget10Hook;
    static Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT> swapChainResizeBuffers10Hook;

    // 
    // D3D11 Hooks
    // 
    static Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, UINT, UINT> swapChainPresent11Hook;
    static Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, const DXGI_MODE_DESC*> swapChainResizeTarget11Hook;
    static Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT> swapChainResizeBuffers11Hook;

    // 
    // D3D12 Hooks
    // 
    static Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, UINT, UINT> swapChainPresent12Hook;
    static Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, const DXGI_MODE_DESC*> swapChainResizeTarget12Hook;
    static Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT> swapChainResizeBuffers12Hook;

#pragma region D3D9

    /*
     * The following section is disabled because hooking IDirect3DDevice9Ex functions
     * should work on "vanilla" D3D9 (like Half-Life 2) equally well while also supporting
     * both windowed and full-screen mode without modifications. Section will be left here
     * for experiments and tests.
     */
#ifdef D3D9_LEGACY_HOOKING

    try
    {
        AutoPtr<Direct3D9Hooking::Direct3D9> d3d(new Direct3D9Hooking::Direct3D9);

        BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9::Present");

        present9Hook.apply(d3d->vtable()[Direct3D9Hooking::Present], [](
            LPDIRECT3DDEVICE9 dev,
            CONST RECT* a1,
            CONST RECT* a2,
            HWND a3,
            CONST RGNDATA* a4
            ) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []()
            {
                Logger::get("HookDX9").information("++ IDirect3DDevice9::Present called");

                INVOKE_INDICIUM_GAME_HOOKED(engine, IndiciumDirect3DVersion9);
            });

            INVOKE_D3D9_CALLBACK(engine, EvtIndiciumD3D9PrePresent, dev, a1, a2, a3, a4);

            auto ret = present9Hook.callOrig(dev, a1, a2, a3, a4);

            INVOKE_D3D9_CALLBACK(engine, EvtIndiciumD3D9PostPresent, dev, a1, a2, a3, a4);

            return ret;
        });

        BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9::Reset");

        reset9Hook.apply(d3d->vtable()[Direct3D9Hooking::Reset], [](
            LPDIRECT3DDEVICE9 dev,
            D3DPRESENT_PARAMETERS* pp
            ) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []() { Logger::get("HookDX9").information("++ IDirect3DDevice9::Reset called"); });

            INVOKE_D3D9_CALLBACK(engine, EvtIndiciumD3D9PreReset, dev, pp);

            auto ret = reset9Hook.callOrig(dev, pp);

            INVOKE_D3D9_CALLBACK(engine, EvtIndiciumD3D9PostReset, dev, pp);

            return ret;
        });

        BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9::EndScene");

        endScene9Hook.apply(d3d->vtable()[Direct3D9Hooking::EndScene], [](
            LPDIRECT3DDEVICE9 dev
            ) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []() { Logger::get("HookDX9").information("++ IDirect3DDevice9::EndScene called"); });

            INVOKE_D3D9_CALLBACK(engine, EvtIndiciumD3D9PreEndScene, dev);

            auto ret = endScene9Hook.callOrig(dev);

            INVOKE_D3D9_CALLBACK(engine, EvtIndiciumD3D9PostEndScene, dev);

            return ret;
        });
    }
    catch (Poco::Exception& pex)
    {
        logger.error("Hooking D3D9 failed: %s", pex.displayText());
    }

#endif

    try
    {
        AutoPtr<Direct3D9Hooking::Direct3D9Ex> d3dEx(new Direct3D9Hooking::Direct3D9Ex);

        BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9Ex::Present";

        present9Hook.apply(d3dEx->vtable()[Direct3D9Hooking::Present], [](
            LPDIRECT3DDEVICE9 dev,
            CONST RECT* a1,
            CONST RECT* a2,
            HWND a3,
            CONST RGNDATA* a4
            ) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []()
            {
                BOOST_LOG_NAMED_SCOPE("HookDX9Ex");
                BOOST_LOG_TRIVIAL(info) << "++ IDirect3DDevice9Ex::Present called";

                INVOKE_INDICIUM_GAME_HOOKED(engine, IndiciumDirect3DVersion9);
            });

            INVOKE_D3D9_CALLBACK(engine, EvtIndiciumD3D9PrePresent, dev, a1, a2, a3, a4);

            const auto ret = present9Hook.call_orig(dev, a1, a2, a3, a4);

            INVOKE_D3D9_CALLBACK(engine, EvtIndiciumD3D9PostPresent, dev, a1, a2, a3, a4);

            return ret;
        });

        BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9Ex::Reset";

        reset9Hook.apply(d3dEx->vtable()[Direct3D9Hooking::Reset], [](
            LPDIRECT3DDEVICE9 dev,
            D3DPRESENT_PARAMETERS* pp
            ) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []()
            {
                BOOST_LOG_NAMED_SCOPE("HookDX9Ex");
                BOOST_LOG_TRIVIAL(info) << "++ IDirect3DDevice9Ex::Reset called";
            });

            INVOKE_D3D9_CALLBACK(engine, EvtIndiciumD3D9PreReset, dev, pp);

            const auto ret = reset9Hook.call_orig(dev, pp);

            INVOKE_D3D9_CALLBACK(engine, EvtIndiciumD3D9PostReset, dev, pp);

            return ret;
        });

        BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9Ex::EndScene";

        endScene9Hook.apply(d3dEx->vtable()[Direct3D9Hooking::EndScene], [](
            LPDIRECT3DDEVICE9 dev
            ) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []()
            {
                BOOST_LOG_NAMED_SCOPE("HookDX9Ex");
                BOOST_LOG_TRIVIAL(info) << "++ IDirect3DDevice9Ex::EndScene called";
            });

            INVOKE_D3D9_CALLBACK(engine, EvtIndiciumD3D9PreEndScene, dev);

            const auto ret = endScene9Hook.call_orig(dev);

            INVOKE_D3D9_CALLBACK(engine, EvtIndiciumD3D9PostEndScene, dev);

            return ret;
        });

        BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9Ex::PresentEx";

        present9ExHook.apply(d3dEx->vtable()[Direct3D9Hooking::PresentEx], [](
            LPDIRECT3DDEVICE9EX dev,
            CONST RECT* a1,
            CONST RECT* a2,
            HWND a3,
            CONST RGNDATA* a4,
            DWORD a5
            ) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []()
            {
                BOOST_LOG_NAMED_SCOPE("HookDX9Ex");
                BOOST_LOG_TRIVIAL(info) << "++ IDirect3DDevice9Ex::PresentEx called";

                INVOKE_INDICIUM_GAME_HOOKED(engine, IndiciumDirect3DVersion9);
            });

            INVOKE_D3D9_CALLBACK(engine, EvtIndiciumD3D9PrePresentEx, dev, a1, a2, a3, a4, a5);

            const auto ret = present9ExHook.call_orig(dev, a1, a2, a3, a4, a5);

            INVOKE_D3D9_CALLBACK(engine, EvtIndiciumD3D9PostPresentEx, dev, a1, a2, a3, a4, a5);

            return ret;
        });

        BOOST_LOG_TRIVIAL(info) << "Hooking IDirect3DDevice9Ex::ResetEx";

        reset9ExHook.apply(d3dEx->vtable()[Direct3D9Hooking::ResetEx], [](
            LPDIRECT3DDEVICE9EX dev,
            D3DPRESENT_PARAMETERS* pp,
            D3DDISPLAYMODEEX* ppp
            ) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []()
            {
                BOOST_LOG_NAMED_SCOPE("HookDX9Ex");
                BOOST_LOG_TRIVIAL(info) << "++ IDirect3DDevice9Ex::ResetEx called";
            });

            INVOKE_D3D9_CALLBACK(engine, EvtIndiciumD3D9PreResetEx, dev, pp, ppp);

            const auto ret = reset9ExHook.call_orig(dev, pp, ppp);

            INVOKE_D3D9_CALLBACK(engine, EvtIndiciumD3D9PostResetEx, dev, pp, ppp);

            return ret;
        });
    }
    catch (DetourException& ex)
    {
        BOOST_LOG_TRIVIAL(error) << "Hooking D3D9Ex failed: " << ex.what();
    }
    catch (ModuleNotFoundException& ex)
    {
        BOOST_LOG_TRIVIAL(warning) << "Module not found: " << ex.what();
    }
    catch (RuntimeException& ex)
    {
        BOOST_LOG_TRIVIAL(error) << "D3D9(Ex) runtime error: " << ex.what();
    }

#pragma endregion

#pragma region D3D10

    static INDICIUM_D3D_VERSION deviceVersion = IndiciumDirect3DVersionUnknown;

    try
    {
        AutoPtr<Direct3D10Hooking::Direct3D10> d3d10(new Direct3D10Hooking::Direct3D10);
        auto vtable = d3d10->vtable();

        BOOST_LOG_TRIVIAL(info) << "Hooking IDXGISwapChain::Present";

        swapChainPresent10Hook.apply(vtable[DXGIHooking::Present], [](
            IDXGISwapChain* chain,
            UINT SyncInterval,
            UINT Flags
            ) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, [&pChain = chain]()
            {
                BOOST_LOG_NAMED_SCOPE("HookDX10");
                BOOST_LOG_TRIVIAL(info) << "++ IDXGISwapChain::Present called";

                ID3D10Device *pp10Device = nullptr;
                ID3D11Device *pp11Device = nullptr;

                auto ret = pChain->GetDevice(__uuidof(ID3D10Device), reinterpret_cast<PVOID*>(&pp10Device));

                if (SUCCEEDED(ret)) {
                    BOOST_LOG_TRIVIAL(debug) << "ID3D10Device object acquired";
                    deviceVersion = IndiciumDirect3DVersion10;
                    INVOKE_INDICIUM_GAME_HOOKED(engine, deviceVersion);
                    return;
                }

                ret = pChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<PVOID*>(&pp11Device));

                if (SUCCEEDED(ret)) {
                    BOOST_LOG_TRIVIAL(debug) << "ID3D11Device object acquired";
                    deviceVersion = IndiciumDirect3DVersion11;
                    INVOKE_INDICIUM_GAME_HOOKED(engine, deviceVersion);
                    return;
                }

                BOOST_LOG_TRIVIAL(error) << "Couldn't fetch device pointer";
            });

            if (deviceVersion == IndiciumDirect3DVersion10) {
                INVOKE_D3D10_CALLBACK(engine, EvtIndiciumD3D10PrePresent, chain, SyncInterval, Flags);
            }

            if (deviceVersion == IndiciumDirect3DVersion11) {
                INVOKE_D3D11_CALLBACK(engine, EvtIndiciumD3D11PrePresent, chain, SyncInterval, Flags);
            }

            const auto ret = swapChainPresent10Hook.call_orig(chain, SyncInterval, Flags);

            if (deviceVersion == IndiciumDirect3DVersion10) {
                INVOKE_D3D10_CALLBACK(engine, EvtIndiciumD3D10PostPresent, chain, SyncInterval, Flags);
            }

            if (deviceVersion == IndiciumDirect3DVersion11) {
                INVOKE_D3D11_CALLBACK(engine, EvtIndiciumD3D11PostPresent, chain, SyncInterval, Flags);
            }

            return ret;
        });

        BOOST_LOG_TRIVIAL(info) << "Hooking IDXGISwapChain::ResizeTarget";

        swapChainResizeTarget10Hook.apply(vtable[DXGIHooking::ResizeTarget], [](
            IDXGISwapChain* chain,
            const DXGI_MODE_DESC* pNewTargetParameters
            ) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []()
            {
                BOOST_LOG_NAMED_SCOPE("HookDX10");
                BOOST_LOG_TRIVIAL(info) << "++ IDXGISwapChain::ResizeTarget called";
            });

            if (deviceVersion == IndiciumDirect3DVersion10) {
                INVOKE_D3D10_CALLBACK(engine, EvtIndiciumD3D10PreResizeTarget, chain, pNewTargetParameters);
            }

            if (deviceVersion == IndiciumDirect3DVersion11) {
                INVOKE_D3D11_CALLBACK(engine, EvtIndiciumD3D11PreResizeTarget, chain, pNewTargetParameters);
            }

            const auto ret = swapChainResizeTarget10Hook.call_orig(chain, pNewTargetParameters);

            if (deviceVersion == IndiciumDirect3DVersion10) {
                INVOKE_D3D10_CALLBACK(engine, EvtIndiciumD3D10PostResizeTarget, chain, pNewTargetParameters);
            }

            if (deviceVersion == IndiciumDirect3DVersion11) {
                INVOKE_D3D11_CALLBACK(engine, EvtIndiciumD3D11PostResizeTarget, chain, pNewTargetParameters);
            }

            return ret;
        });

        BOOST_LOG_TRIVIAL(info) << "Hooking IDXGISwapChain::ResizeBuffers";

        swapChainResizeBuffers10Hook.apply(vtable[DXGIHooking::ResizeBuffers], [](
            IDXGISwapChain* chain,
            UINT            BufferCount,
            UINT            Width,
            UINT            Height,
            DXGI_FORMAT     NewFormat,
            UINT            SwapChainFlags
            ) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []()
            {
                BOOST_LOG_NAMED_SCOPE("HookDX10");
                BOOST_LOG_TRIVIAL(info) << "++ IDXGISwapChain::ResizeBuffers called";
            });

            if (deviceVersion == IndiciumDirect3DVersion10) {
                INVOKE_D3D10_CALLBACK(engine, EvtIndiciumD3D10PreResizeBuffers, chain,
                    BufferCount, Width, Height, NewFormat, SwapChainFlags);
            }

            if (deviceVersion == IndiciumDirect3DVersion11) {
                INVOKE_D3D11_CALLBACK(engine, EvtIndiciumD3D11PreResizeBuffers, chain,
                    BufferCount, Width, Height, NewFormat, SwapChainFlags);
            }

            const auto ret = swapChainResizeBuffers10Hook.call_orig(chain,
                BufferCount, Width, Height, NewFormat, SwapChainFlags);

            if (deviceVersion == IndiciumDirect3DVersion10) {
                INVOKE_D3D10_CALLBACK(engine, EvtIndiciumD3D10PostResizeBuffers, chain,
                    BufferCount, Width, Height, NewFormat, SwapChainFlags);
            }

            if (deviceVersion == IndiciumDirect3DVersion11) {
                INVOKE_D3D11_CALLBACK(engine, EvtIndiciumD3D11PostResizeBuffers, chain,
                    BufferCount, Width, Height, NewFormat, SwapChainFlags);
            }

            return ret;
        });
    }
    catch (DetourException& ex)
    {
        BOOST_LOG_TRIVIAL(error) << "Hooking D3D10 failed: " << ex.what();
    }
    catch (ModuleNotFoundException& ex)
    {
        BOOST_LOG_TRIVIAL(warning) << "Module not found: " << ex.what();
    }
    catch (RuntimeException& ex)
    {
        BOOST_LOG_TRIVIAL(error) << "D3D10 runtime error: " << ex.what();
    }

#pragma endregion

#pragma region D3D11

    try
    {
        AutoPtr<Direct3D11Hooking::Direct3D11> d3d11(new Direct3D11Hooking::Direct3D11);
        auto vtable = d3d11->vtable();

        BOOST_LOG_TRIVIAL(info) << "Hooking IDXGISwapChain::Present";

        swapChainPresent11Hook.apply(vtable[DXGIHooking::Present], [](
            IDXGISwapChain* chain,
            UINT SyncInterval,
            UINT Flags
            ) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []()
            {
                BOOST_LOG_NAMED_SCOPE("HookDX11");
                BOOST_LOG_TRIVIAL(info) << "++ IDXGISwapChain::Present called";

                INVOKE_INDICIUM_GAME_HOOKED(engine, IndiciumDirect3DVersion11);
            });

            INVOKE_D3D11_CALLBACK(engine, EvtIndiciumD3D11PrePresent, chain, SyncInterval, Flags);

            const auto ret = swapChainPresent11Hook.call_orig(chain, SyncInterval, Flags);

            INVOKE_D3D11_CALLBACK(engine, EvtIndiciumD3D11PostPresent, chain, SyncInterval, Flags);

            return ret;
        });

        BOOST_LOG_TRIVIAL(info) << "Hooking IDXGISwapChain::ResizeTarget";

        swapChainResizeTarget11Hook.apply(vtable[DXGIHooking::ResizeTarget], [](
            IDXGISwapChain* chain,
            const DXGI_MODE_DESC* pNewTargetParameters
            ) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []()
            {
                BOOST_LOG_NAMED_SCOPE("HookDX11");
                BOOST_LOG_TRIVIAL(info) << "++ IDXGISwapChain::ResizeTarget called";
            });

            INVOKE_D3D11_CALLBACK(engine, EvtIndiciumD3D11PreResizeTarget, chain, pNewTargetParameters);

            const auto ret = swapChainResizeTarget11Hook.call_orig(chain, pNewTargetParameters);

            INVOKE_D3D11_CALLBACK(engine, EvtIndiciumD3D11PostResizeTarget, chain, pNewTargetParameters);

            return ret;
        });

        BOOST_LOG_TRIVIAL(info) << "Hooking IDXGISwapChain::ResizeBuffers";

        swapChainResizeBuffers11Hook.apply(vtable[DXGIHooking::ResizeBuffers], [](
            IDXGISwapChain* chain,
            UINT            BufferCount,
            UINT            Width,
            UINT            Height,
            DXGI_FORMAT     NewFormat,
            UINT            SwapChainFlags
            ) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []()
            {
                BOOST_LOG_NAMED_SCOPE("HookDX11");
                BOOST_LOG_TRIVIAL(info) << "++ IDXGISwapChain::ResizeBuffers called";
            });

            INVOKE_D3D11_CALLBACK(engine, EvtIndiciumD3D11PreResizeBuffers, chain,
                BufferCount, Width, Height, NewFormat, SwapChainFlags);

            const auto ret = swapChainResizeBuffers11Hook.call_orig(chain,
                BufferCount, Width, Height, NewFormat, SwapChainFlags);

            INVOKE_D3D11_CALLBACK(engine, EvtIndiciumD3D11PostResizeBuffers, chain,
                BufferCount, Width, Height, NewFormat, SwapChainFlags);

            return ret;
        });
    }
    catch (DetourException& ex)
    {
        BOOST_LOG_TRIVIAL(error) << "Hooking D3D11 failed: " << ex.what();
    }
    catch (ModuleNotFoundException& ex)
    {
        BOOST_LOG_TRIVIAL(warning) << "Module not found: " << ex.what();
    }
    catch (RuntimeException& ex)
    {
        BOOST_LOG_TRIVIAL(error) << "D3D11 runtime error: " << ex.what();
    }

#pragma endregion

#pragma region D3D12

    try
    {
        AutoPtr<Direct3D12Hooking::Direct3D12> d3d12(new Direct3D12Hooking::Direct3D12);
        auto vtable = d3d12->vtable();

        BOOST_LOG_TRIVIAL(info) << "Hooking IDXGISwapChain::Present";

        swapChainPresent12Hook.apply(vtable[DXGIHooking::Present], [](
            IDXGISwapChain* chain,
            UINT SyncInterval,
            UINT Flags
            ) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []()
            {
                BOOST_LOG_NAMED_SCOPE("HookDX12");
                BOOST_LOG_TRIVIAL(info) << "++ IDXGISwapChain::Present called";

                INVOKE_INDICIUM_GAME_HOOKED(engine, IndiciumDirect3DVersion12);
            });

            INVOKE_D3D12_CALLBACK(engine, EvtIndiciumD3D12PrePresent, chain, SyncInterval, Flags);

            const auto ret = swapChainPresent12Hook.call_orig(chain, SyncInterval, Flags);

            INVOKE_D3D12_CALLBACK(engine, EvtIndiciumD3D12PostPresent, chain, SyncInterval, Flags);

            return ret;
        });

        BOOST_LOG_TRIVIAL(info) << "Hooking IDXGISwapChain::ResizeTarget";

        swapChainResizeTarget12Hook.apply(vtable[DXGIHooking::ResizeTarget], [](
            IDXGISwapChain* chain,
            const DXGI_MODE_DESC* pNewTargetParameters
            ) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []()
            {
                BOOST_LOG_NAMED_SCOPE("HookDX12");
                BOOST_LOG_TRIVIAL(info) << "++ IDXGISwapChain::ResizeTarget called";
            });

            INVOKE_D3D12_CALLBACK(engine, EvtIndiciumD3D12PreResizeTarget, chain, pNewTargetParameters);

            const auto ret = swapChainResizeTarget12Hook.call_orig(chain, pNewTargetParameters);

            INVOKE_D3D12_CALLBACK(engine, EvtIndiciumD3D12PostResizeTarget, chain, pNewTargetParameters);

            return ret;
        });

        BOOST_LOG_TRIVIAL(info) << "Hooking IDXGISwapChain::ResizeBuffers";

        swapChainResizeBuffers12Hook.apply(vtable[DXGIHooking::ResizeBuffers], [](
            IDXGISwapChain* chain,
            UINT            BufferCount,
            UINT            Width,
            UINT            Height,
            DXGI_FORMAT     NewFormat,
            UINT            SwapChainFlags
            ) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []()
            {
                BOOST_LOG_NAMED_SCOPE("HookDX12");
                BOOST_LOG_TRIVIAL(info) << "++ IDXGISwapChain::ResizeBuffers called";
            });

            INVOKE_D3D12_CALLBACK(engine, EvtIndiciumD3D12PreResizeBuffers, chain,
                BufferCount, Width, Height, NewFormat, SwapChainFlags);

            const auto ret = swapChainResizeBuffers12Hook.call_orig(chain,
                BufferCount, Width, Height, NewFormat, SwapChainFlags);

            INVOKE_D3D12_CALLBACK(engine, EvtIndiciumD3D12PostResizeBuffers, chain,
                BufferCount, Width, Height, NewFormat, SwapChainFlags);

            return ret;
        });
    }
    catch (DetourException& ex)
    {
        BOOST_LOG_TRIVIAL(error) << "Hooking D3D12 failed: " << ex.what();
    }
    catch (ModuleNotFoundException& ex)
    {
        BOOST_LOG_TRIVIAL(warning) << "Module not found: " << ex.what();
    }
    catch (RuntimeException& ex)
    {
        BOOST_LOG_TRIVIAL(error) << "D3D12 runtime error: " << ex.what();
    }

#pragma endregion

#ifdef HOOK_DINPUT8
    //
    // TODO: legacy, fix me up!
    // 
    if (engine->Configuration->getBool("DInput8.enabled", false))
    {
        bool dinput8_available;
        size_t vtable8[DirectInput8Hooking::DirectInput8::VTableElements] = { 0 };

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
            BOOST_LOG_TRIVIAL(info) << "Game uses DirectInput8");
            HookDInput8(vtable8);
        }
    }
#endif

    BOOST_LOG_TRIVIAL(info) << "Library initialized successfully";

    //
    // Wait until cancellation requested
    // 
    WaitForSingleObject(engine->EngineCancellationEvent, INFINITE);

    BOOST_LOG_TRIVIAL(info) << "Shutting down hooks...";

    try
    {
        present9Hook.remove();
        reset9Hook.remove();
        endScene9Hook.remove();
        present9ExHook.remove();
        reset9ExHook.remove();
        swapChainPresent10Hook.remove();
        swapChainResizeTarget10Hook.remove();
        swapChainResizeBuffers10Hook.remove();
        swapChainPresent11Hook.remove();
        swapChainResizeTarget11Hook.remove();
        swapChainResizeBuffers11Hook.remove();
        swapChainPresent12Hook.remove();
        swapChainResizeTarget12Hook.remove();
        swapChainResizeBuffers12Hook.remove();

        BOOST_LOG_TRIVIAL(info) << "Hooks disabled";
    }
    catch (DetourException& pex)
    {
        BOOST_LOG_TRIVIAL(error) << "Unhooking failed: " << pex.what();
    }

    //
    // Inform caller that it's safe to continue
    // 
    SetEvent(engine->EngineCancellationCompletedEvent);

    return 0;
}

#ifdef HOOK_DINPUT8
void HookDInput8(size_t* vtable8)
{
    auto& logger = Logger::get(__func__);
    BOOST_LOG_TRIVIAL(info) << "Hooking IDirectInputDevice8::Acquire");

    g_acquire8Hook.apply(vtable8[DirectInput8Hooking::Acquire], [](LPDIRECTINPUTDEVICE8 dev) -> HRESULT
    {
        static std::once_flag flag;
        std::call_once(flag, []() { Logger::get("HookDInput8").information("++ IDirectInputDevice8::Acquire called"); });

        return g_acquire8Hook.callOrig(dev);
    });

    BOOST_LOG_TRIVIAL(info) << "Hooking IDirectInputDevice8::GetDeviceData");

    g_getDeviceData8Hook.apply(vtable8[DirectInput8Hooking::GetDeviceData], [](LPDIRECTINPUTDEVICE8 dev, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags) -> HRESULT
    {
        static std::once_flag flag;
        std::call_once(flag, []() { Logger::get("HookDInput8").information("++ IDirectInputDevice8::Acquire called"); });

        return g_getDeviceData8Hook.callOrig(dev, cbObjectData, rgdod, pdwInOut, dwFlags);
    });

    BOOST_LOG_TRIVIAL(info) << "Hooking IDirectInputDevice8::GetDeviceInfo");

    g_getDeviceInfo8Hook.apply(vtable8[DirectInput8Hooking::GetDeviceInfo], [](LPDIRECTINPUTDEVICE8 dev, LPDIDEVICEINSTANCE pdidi) -> HRESULT
    {
        static std::once_flag flag;
        std::call_once(flag, []() { Logger::get("HookDInput8").information("++ IDirectInputDevice8::GetDeviceInfo called"); });

        return g_getDeviceInfo8Hook.callOrig(dev, pdidi);
    });

    BOOST_LOG_TRIVIAL(info) << "Hooking IDirectInputDevice8::GetDeviceState");

    g_getDeviceState8Hook.apply(vtable8[DirectInput8Hooking::GetDeviceState], [](LPDIRECTINPUTDEVICE8 dev, DWORD cbData, LPVOID lpvData) -> HRESULT
    {
        static std::once_flag flag;
        std::call_once(flag, []() { Logger::get("HookDInput8").information("++ IDirectInputDevice8::GetDeviceState called"); });

        return g_getDeviceState8Hook.callOrig(dev, cbData, lpvData);
    });

    BOOST_LOG_TRIVIAL(info) << "Hooking IDirectInputDevice8::GetObjectInfo");

    g_getObjectInfo8Hook.apply(vtable8[DirectInput8Hooking::GetObjectInfo], [](LPDIRECTINPUTDEVICE8 dev, LPDIDEVICEOBJECTINSTANCE pdidoi, DWORD dwObj, DWORD dwHow) -> HRESULT
    {
        static std::once_flag flag;
        std::call_once(flag, []() { Logger::get("HookDInput8").information("++ IDirectInputDevice8::GetObjectInfo called"); });

        return g_getObjectInfo8Hook.callOrig(dev, pdidoi, dwObj, dwHow);
    });
}
#endif