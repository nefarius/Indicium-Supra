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

The core DLL can be injected in any DirectX-based game/process (32-Bit or 64-Bit) and does all of the heavy lifting automatically.

## Prerequisites
 * Visual Studio **2015** ([Community Edition](https://go.microsoft.com/fwlink/p/?LinkId=534599) is just fine)
   * If asked, install Windows XP support for C++
   ![](https://lh3.googleusercontent.com/-jPbvojP6oeA/WYySR-CS9XI/AAAAAAAAALE/4dpA1-48OdA9EXHrWK7kCbEW_0vOY13VQCHMYCw/s0/devenv_2017-08-10_19-05-09.png)
 * [Microsoft DirectX SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
 * [POCO C++ libraries](https://pocoproject.org/) (referenced by NuGet)
 * [MinHook](https://github.com/TsudaKageyu/minhook) (referenced by NuGet)
 
## Supported DirectX versions
 * DirectX 9.0
 * DirectX 9.0 Extended
 * DirectX 10
 * DirectX 11

## How to build
Building should be pretty straight-forward since the POCO libraries get fetched [pre-compiled via NuGet](https://www.nuget.org/packages/poco-basic-windows-v140/) on first build automatically. The only manual additional step is to install the DirectX SDK.

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
