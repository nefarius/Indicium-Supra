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
