#pragma once

#include <vector>
#include <map>
#include <Indicium/Plugin/Common.h>

#define POCO_NO_UNWINDOWS
#include <Poco/SharedLibrary.h>
#include <Poco/Mutex.h>
#include <d3d9.h>
#include <dxgi.h>

using Poco::SharedLibrary;
using Poco::FastMutex;
using Poco::ScopedLock;

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

class PluginManager
{
    TCHAR m_DllPath[MAX_PATH];
    std::vector<SharedLibrary*> plugins;
    FastMutex mPlugins;

    std::map<std::string, std::vector<LPVOID>> _fpMap;
    std::vector<std::string> exports = { "Present", "Reset", "EndScene", "ResizeTarget" };

    std::vector<std::string> d3d9_exports = { "" };

public:
    PluginManager();
    ~PluginManager();

    void load();
    void unload();
    void present(IID guid, LPVOID unknown, Direct3DVersion version);
    void reset(IID guid, LPVOID unknown, Direct3DVersion version);
    void endScene(IID guid, LPVOID unknown, Direct3DVersion version);
    void resizeTarget(IID guid, LPVOID unknown, Direct3DVersion version);

    void load(Direct3DVersion version);

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

