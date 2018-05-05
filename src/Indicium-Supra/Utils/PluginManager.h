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

#pragma once

#include <vector>
#include <map>

#include <Indicium/Plugin/Common.h>

#include <d3d9.h>
#include <dxgi.h>

#define POCO_NO_UNWINDOWS
#include <Poco/SharedLibrary.h>
#include <Poco/Mutex.h>
#include <Poco/SharedPtr.h>
#include <Poco/RefCountedObject.h>
#include <Poco/AutoPtr.h>

using Poco::SharedLibrary;
using Poco::FastMutex;
using Poco::ScopedLock;
using Poco::SharedPtr;
using Poco::RefCountedObject;
using Poco::AutoPtr;

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

class PluginManager
{
    //
    // D3D9(Ex)
    // 
    typedef HRESULT(__cdecl* fp_d3d9_present)(LPDIRECT3DDEVICE9, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *);
    typedef HRESULT(__cdecl* fp_d3d9_reset)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS *);
    typedef HRESULT(__cdecl* fp_d3d9_endscene)(LPDIRECT3DDEVICE9);
    typedef HRESULT(__cdecl* fp_d3d9_presentex)(LPDIRECT3DDEVICE9EX, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *, DWORD);
    typedef HRESULT(__cdecl* fp_d3d9_resetex)(LPDIRECT3DDEVICE9EX, D3DPRESENT_PARAMETERS *, D3DDISPLAYMODEEX *);

    //
    // DXGI
    // 
    typedef HRESULT(__cdecl* fp_dxgi_present)(IDXGISwapChain*, UINT, UINT);
    typedef HRESULT(__cdecl *fp_dxgi_resizetarget)(IDXGISwapChain*, const DXGI_MODE_DESC*);

    //
    // Plugin-specific
    // 
    typedef BOOLEAN (__cdecl* fp_init)(Direct3DVersion);

    //
    // Plugin object
    // 
    struct Plugin : RefCountedObject
    {
        Direct3DVersion version;
        SharedPtr<SharedLibrary> plugin_module;
        fp_init init;
        union
        {
            struct 
            {
                fp_d3d9_present present;
                fp_d3d9_reset reset;
                fp_d3d9_endscene endscene;
                fp_d3d9_presentex presentex;
                fp_d3d9_resetex resetex;
            } d3d9;
            struct
            {
                fp_dxgi_present present;
                fp_dxgi_resizetarget resizetarget;
            } dxgi;
        } function_ptrs;
    };

    TCHAR _dll_path[MAX_PATH];
    FastMutex _plugins_lock;
    std::vector<AutoPtr<Plugin>> _plugins;

public:
    PluginManager();
    ~PluginManager();
        
    void load(Direct3DVersion version);
    void unload();

    void d3d9_present(
        LPDIRECT3DDEVICE9   pDevice,
        const RECT          *pSourceRect,
        const RECT          *pDestRect,
        HWND                hDestWindowOverride,
        const RGNDATA       *pDirtyRegion
    );
    void d3d9_reset(
        LPDIRECT3DDEVICE9       pDevice,
        D3DPRESENT_PARAMETERS   *pPresentationParameters
    );
    void d3d9_endscene(
        LPDIRECT3DDEVICE9 pDevice
    );

    void d3d9_presentex(
        LPDIRECT3DDEVICE9EX     pDevice,
        const RECT              *pSourceRect,
        const RECT              *pDestRect,
        HWND                    hDestWindowOverride,
        const RGNDATA           *pDirtyRegion,
        DWORD                   dwFlags
    );
    void d3d9_resetex(
        LPDIRECT3DDEVICE9EX     pDevice,
        D3DPRESENT_PARAMETERS   *pPresentationParameters,
        D3DDISPLAYMODEEX        *pFullscreenDisplayMode
    );

    void d3d10_present(
        IDXGISwapChain  *pSwapChain,
        UINT            SyncInterval,
        UINT            Flags
    );
    void d3d10_resizetarget(
        IDXGISwapChain          *pSwapChain,
        const DXGI_MODE_DESC    *pNewTargetParameters
    );

    void d3d11_present(
        IDXGISwapChain  *pSwapChain,
        UINT            SyncInterval,
        UINT            Flags
    );
    void d3d11_resizetarget(
        IDXGISwapChain          *pSwapChain,
        const DXGI_MODE_DESC    *pNewTargetParameters
    );

    void d3d12_present(
        IDXGISwapChain  *pSwapChain,
        UINT            SyncInterval,
        UINT            Flags
    );
    void d3d12_resizetarget(
        IDXGISwapChain          *pSwapChain,
        const DXGI_MODE_DESC    *pNewTargetParameters
    );
};

