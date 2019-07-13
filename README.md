# Indicium-Supra

API-Hooking and rendering framework for DirectX-based games.

[![Build status](https://ci.appveyor.com/api/projects/status/rt4ybpwrhn22kegm?svg=true)](https://ci.appveyor.com/project/nefarius/indicium-supra) [![Discord](https://img.shields.io/discord/346756263763378176.svg)](https://discord.vigem.org) [![Website](https://img.shields.io/website-up-down-green-red/https/vigem.org.svg?label=ViGEm.org)](https://vigem.org/) [![PayPal Donate](https://img.shields.io/badge/paypal-donate-blue.svg)](<https://paypal.me/NefariusMaximus>) [![Support on Patreon](https://img.shields.io/badge/patreon-donate-orange.svg)](<https://www.patreon.com/nefarius>) [![GitHub followers](https://img.shields.io/github/followers/nefarius.svg?style=social&label=Follow)](https://github.com/nefarius) [![Twitter Follow](https://img.shields.io/twitter/follow/nefariusmaximus.svg?style=social&label=Follow)](https://twitter.com/nefariusmaximus)

## About

`Indicium-Supra` consists of a self-contained library (DLL) which exposes a minimalistic API for rendering custom content in foreign processes eliminating the need for in-depth knowledge about Direct3D and API-hooking. The most common use-case might be drawing custom overlays on top of your games. The framework takes care about pesky tasks like detecting the DirectX version the game was built for and supports runtime-hooking (no special launcher application required).

## Supported DirectX versions

- DirectX 9.0
- DirectX 9.0 Extended (Vista+)
- DirectX 10
- DirectX 11
- DirectX 12 (implemented but untested)

## How to build

### Prerequisites

- Visual Studio **2017** ([Community Edition](https://www.visualstudio.com/thank-you-downloading-visual-studio/?sku=Community&rel=15) is just fine)
- [Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk)
- [.NET Core SDK 2.1](https://www.microsoft.com/net/download/dotnet-core/2.1) (or greater, required for building via CLI only)
- [Follow the Vcpkg Quick Start](https://github.com/Microsoft/vcpkg#quick-start) and install the following packages:
  - `.\vcpkg install spdlog:x86-windows-static spdlog:x64-windows-static detours:x86-windows-static detours:x64-windows-static`
  - For the ImGui sample to build you'll also need:
    - `.\vcpkg install imgui:x86-windows-static imgui:x64-windows-static`

Building should be pretty straight-forward since the dependencies get installed via [Vcpkg](https://github.com/Microsoft/vcpkg). You have multiple choices for getting things done.

### Visual Studio

Just open the solution file `Indicium-Supra.sln` and start the build from there.

### Command Line

Navigate to the solution directory on the command line and invoke:

```PowerShell
powershell .\build.ps1
```

### The lazy way

Now if you're really in a hurry you can [grab pre-built binaries from the buildbot](https://buildbot.vigem.org/builds/Indicium-Supra/master/). Boom, done.

## How to use

Inject the resulting host library (e.g. `Indicium-ImGui.dll`) into the target process first using a DLL injection utility of your choice (you can ofc. [use mine as well](https://github.com/nefarius/Injector)). The following example loads the [imgui sample](samples/Indicium-ImGui):

```PowerShell
.\Injector -i -n hl2.exe Indicium-ImGui.dll
```

Just make sure your host library doesn't require any external dependencies not present in the process context or you'll get a `LoadLibrary failed` error.

## Diagnostics

The core library logs its progress and potential errors to the file `%TEMP%\Indicium-Supra.log`.

## Demos

The following screenshots show [imgui](https://github.com/ocornut/imgui) getting rendered in foreign processes using different versions of DirectX.

### DirectX 9

Half-Life 2, 32-Bit

![Half-Life 2](https://thumbs.gfycat.com/AltruisticElectricIberianmole-size_restricted.gif)

### DirectX 9 Ex

Castlevania: Lords of Shadow, 32-Bit

![Castlevania: Lords of Shadow](https://thumbs.gfycat.com/DeafeningSomeKob-size_restricted.gif)

### DirectX 10

Bioshock 2, 32-Bit

![Bioshock2_2017-08-10_14-48-38.png](https://lh3.googleusercontent.com/-oWNHhCbWBLo/WYxWJ0l6qqI/AAAAAAAAAK0/2yTiRcSeH-I-6YUmsGfPimBSti4VobfQgCHMYCw/s0/Bioshock2_2017-08-10_14-48-38.png)

### DirectX 11

Ryse: Son of Rome, 64-Bit

![Ryse_2017-08-09_21-48-57.png](https://lh3.googleusercontent.com/-A1Yj4RE07C4/WYtnLFgGg3I/AAAAAAAAAKk/ZMEfZNAQ670XR4vtRtO0Yy3vN2EXwXKygCHMYCw/s0/Ryse_2017-08-09_21-48-57.png)

Road Redemption, 64-Bit

![RoadRedemptionEarlyAccess_2017-08-14_14-06-21.png](https://lh3.googleusercontent.com/-v2Ao0kCoTvg/WZGSQeniK8I/AAAAAAAAAN0/TXpCVv8pLFoGsuKlwnBRufFuG_ZXaZmRACHMYCw/s0/RoadRedemptionEarlyAccess_2017-08-14_14-06-21.png)

## Sources

- [DX9-Overlay-API](https://github.com/agrippa1994/DX9-Overlay-API)
- [CREATING A RENDER TARGET IN DIRECTX11](http://www.hlsl.co.uk/blog/2014/11/19/creating-a-render-target-in-directx11)
- [ion RE Library](https://github.com/scen/ionlib)
- [C# – SCREEN CAPTURE AND OVERLAYS FOR DIRECT3D 9, 10 AND 11 USING API HOOKS](http://spazzarama.com/2011/03/14/c-screen-capture-and-overlays-for-direct3d-9-10-and-11-using-api-hooks/)
- [HelloD3D12](https://github.com/GPUOpen-LibrariesAndSDKs/HelloD3D12)
