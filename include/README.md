# Plugin development

**DEPRECATED! Fix and/or move to Wiki!**

The actual rendering within the target process is done by plugins. Developing a plugin is very simple, just follow these steps to get a basic skeleton:

Create a new `Dynamic-Link Library (DLL)`:

![](https://lh3.googleusercontent.com/-AY_ia4E8mZk/Wu4MJRwZJII/AAAAAAAABLA/vi1bH1yX97UanGXOjMDzXGKtqw2Mof2xgCHMYCw/s0/devenv_2018-05-05_21-55-17.png)

All you need to do is add the `include/` path to the project:

![](https://lh3.googleusercontent.com/-CcHpmUNvAsE/Wu4KwAnaQ7I/AAAAAAAABKs/4et9xbqxja0BXtMWDzQkfAqvPTrjFA04QCHMYCw/s0/devenv_2018-05-05_21-49-19.png)

Here's a basic template to hook into the `Present` function of the D3D9 device:

```C
// Only include necessary; this sample targets Direct3D 9.0
#include <Indicium/Plugin/Direct3D9.h>

// Reference the DirectX libraries here or in project settings
#pragma comment(lib, "D3dx9.lib")

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
    //
    // We don't need to get notified in thread attach- or detachments
    // 
    DisableThreadLibraryCalls(static_cast<HMODULE>(hInstance));

    return TRUE;
}

//
// Report back to core library that we can run in a game using D3D9
//
INDICIUM_EXPORT(BOOLEAN) indicium_plugin_init(Direct3DVersion version)
{
    return (version == Direct3DVersion::Direct3D9);
}

INDICIUM_EXPORT(VOID) indicium_plugin_d3d9_present(
    LPDIRECT3DDEVICE9   pDevice,
    const RECT          *pSourceRect,
    const RECT          *pDestRect,
    HWND                hDestWindowOverride,
    const RGNDATA       *pDirtyRegion
)
{
    D3DDEVICE_CREATION_PARAMETERS params;

    auto hr = pDevice->GetCreationParameters(&params);

    // Do whatever you like to do
}
```
