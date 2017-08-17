# Indicium-Supra

API-Hooking and rendering framework for DirectX-based games.

## Summary
`Indicium-Supra` exposes the following DirectX/Direct3D rendering COM functions to custom plugins:
 * `IDirect3DDevice9::Present`
 * `IDirect3DDevice9::Reset`
 * `IDirect3DDevice9::EndScene`
 * `IDirect3DDevice9Ex::PresentEx`
 * `IDirect3DDevice9Ex::ResetEx`
 * `IDXGISwapChain::Present`
 * `IDXGISwapChain::ResizeTarget`

The core DLL can be injected in any DirectX-based game/process (32-Bit or 64-Bit) and does all of the heavy lifting automatically. On loading it tries to detect the used DirectX/Direct3D version, acquire the virtual function pointer table and hook into all common functions used for rendering. Every time the host process renders content, the function calls get intercepted and forwarded to one or more plugins which also get loaded on library boot. The plugins can then do what they want with the provided device/swapchain pointers (like render additional content or blank out certain parts of the resulting image).

## Prerequisites
 * Visual Studio **2017** ([Community Edition](https://www.visualstudio.com/thank-you-downloading-visual-studio/?sku=Community&rel=15) is just fine)
 * [Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk)
 * [POCO C++ libraries](https://pocoproject.org/) (referenced by NuGet)
 * [MinHook](https://github.com/TsudaKageyu/minhook) (referenced by NuGet)
 
## Supported DirectX versions
 * DirectX 9.0
 * DirectX 9.0 Extended (Vista+)
 * DirectX 10
 * DirectX 11
 * DirectX 12

## How to build
Building should be pretty straight-forward since the POCO libraries get fetched [pre-compiled via NuGet](https://www.nuget.org/packages/poco-basic-windows-v140/) on first build automatically. The only manual additional step is to install the DirectX SDK.

If Visual Studio can't load one or more projects the first time you launch ist, you might need to restore the NuGet packages. Open the Package Manager Console and hit Restore:

![](https://lh3.googleusercontent.com/-K6g4v1RNMQo/WYyUwkquvZI/AAAAAAAAALQ/G_njXRtZQmwZUmo210vcLN_3pJphOuNigCHMYCw/s0/devenv_2017-08-10_19-15-45.png)

## How to use
Inject the resulting `Indicium-Supra.dll` into the target process using a DLL injection utility of your choice (you can ofc. [use mine as well](https://github.com/nefarius/Injector)). To do anything usefull you also need one or more plugins in the same directory as well. The plugin DLLs names have to end with `.Plugin.dll` to get auto-detected and loaded. Example:

```Bash
Injector --inject --module-name Indicium-Supra.dll --process-name hl2.exe
```

## Diagnostics
The core library logs its progress and potential errors to the file `%TEMP%\Indicium-Supra.log`.

## Demos
The following screenshots show [imgui](https://github.com/ocornut/imgui) getting rendered in foreign processes using different versions of DirectX.
### DirectX 9
Half-Life 2, 32-Bit

![](https://lh3.googleusercontent.com/-Xxo-6C13lLQ/WYtg-8solMI/AAAAAAAAAKI/DpJAtgbemjUosoeGgVu4nTh4dMW52rweACHMYCw/s0/hl2_2017-08-09_21-22-31.png)

### DirectX 9 Ex
Castlevania: Lords of Shadow, 32-Bit

![](https://lh3.googleusercontent.com/-lzrH1cvY-io/WYtiMBHjoOI/AAAAAAAAAKU/sghbja-iE4ML7hCJPJ_7NomW5UxlDvREwCHMYCw/s0/CastlevaniaLoSUE_2017-08-09_21-27-44.jpg)

### DirectX 10
Bioshock 2, 32-Bit

![](https://lh3.googleusercontent.com/-oWNHhCbWBLo/WYxWJ0l6qqI/AAAAAAAAAK0/2yTiRcSeH-I-6YUmsGfPimBSti4VobfQgCHMYCw/s0/Bioshock2_2017-08-10_14-48-38.png)

### DirectX 11
Ryse: Son of Rome, 64-Bit

![](https://lh3.googleusercontent.com/-A1Yj4RE07C4/WYtnLFgGg3I/AAAAAAAAAKk/ZMEfZNAQ670XR4vtRtO0Yy3vN2EXwXKygCHMYCw/s0/Ryse_2017-08-09_21-48-57.png)

Road Redemption, 64-Bit

![](https://lh3.googleusercontent.com/-v2Ao0kCoTvg/WZGSQeniK8I/AAAAAAAAAN0/TXpCVv8pLFoGsuKlwnBRufFuG_ZXaZmRACHMYCw/s0/RoadRedemptionEarlyAccess_2017-08-14_14-06-21.png)

## Sources
 * [CREATING A RENDER TARGET IN DIRECTX11](http://www.hlsl.co.uk/blog/2014/11/19/creating-a-render-target-in-directx11)
 * [ion RE Library](https://github.com/scen/ionlib)
 * [C# – SCREEN CAPTURE AND OVERLAYS FOR DIRECT3D 9, 10 AND 11 USING API HOOKS](http://spazzarama.com/2011/03/14/c-screen-capture-and-overlays-for-direct3d-9-10-and-11-using-api-hooks/)
 * [HelloD3D12](https://github.com/GPUOpen-LibrariesAndSDKs/HelloD3D12)
