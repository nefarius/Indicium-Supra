# Plugin development

The actual rendering within the target process is done by plugins. Developing a plugin is very simple, just follow these steps to get a basic skeleton:

Create a new `Win32 Project`:

  ![](https://lh3.googleusercontent.com/-sYOox5eGUH4/WYzTuNABj5I/AAAAAAAAALs/qQr4ZbJk4nkpme1pS7kWIYnjoKM0-W-rwCHMYCw/s0/devenv_2017-08-10_23-44-20.png)

Next:
 
![](https://lh3.googleusercontent.com/-yE9ORZ8LlTw/WYzT0hewUpI/AAAAAAAAALw/XUSzN6dCSU4EddkwKgK0dnOKlQl7g08EwCHMYCw/s0/devenv_2017-08-10_23-44-49.png)

Create an empty DLL project:
 
![](https://lh3.googleusercontent.com/-7yUfoGKj_v0/WYzT8zsQ3oI/AAAAAAAAAL0/txqg51o6PUQb8QrrCnJFmc6R30d0VVEXgCHMYCw/s0/devenv_2017-08-10_23-45-22.png)

Add a source file for your entry point:

![](https://lh3.googleusercontent.com/-vq0tn80rncU/WYzUSRa8pPI/AAAAAAAAAL4/InJ5r3A8VloprcCkU-GD9850jikYNaszgCHMYCw/s0/devenv_2017-08-10_23-46-48.png)

In this example we utilize the DirectX SDK to access D3D9 types:

![](https://lh3.googleusercontent.com/-30r-r9lHG_o/WYzV9CJ0dLI/AAAAAAAAAME/Kak1ZGepESIudHEn8jIVGhtGZRtUQ6teACHMYCw/s0/devenv_2017-08-10_23-53-55.png)

Here's a basic template to hook into the `Present` function of the D3D9 device:

```C
// Only include necessary
#include "../../Indicium-Supra/include/IndiciumPlugin.h"

// This sample targets Direct3D 9.0
#include <d3d9.h>
#include <D3dx9core.h>
#pragma comment(lib, "D3dx9.lib")

// DllMain boilerplate
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

// Present gets called by Indicium-Supra
INDICIUM_EXPORT Present(IID guid, LPVOID unknown, Direct3DVersion version)
{
    if (guid == IID_IDirect3DDevice9)
    {
        auto pd3dDevice = static_cast<IDirect3DDevice9*>(unknown);

        D3DDEVICE_CREATION_PARAMETERS params;

        auto hr = pd3dDevice->GetCreationParameters(&params);

        // Do whatever you like to do
    }
}
```
