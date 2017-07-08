# Indicium-Supra (LGPL v3)

An overlay API for DirectX based games, which is licensed and distributed under the terms of the LGPL v3

## Prerequisites
 * Visual Studio **2015** ([Community Edition](https://go.microsoft.com/fwlink/p/?LinkId=534599) is just fine)
   * [Visual Studio 2017 currently can't build the boost libraries](https://www.reddit.com/r/cpp/comments/5ut2bw/visual_studio_2017_isnt_supported_by_boost/)
 * [Microsoft DirectX SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
 * [Boost libraries](http://www.boost.org/)
 * [MinHook](https://github.com/TsudaKageyu/minhook)

## How to build
### Build Boost Libraries
 * Download and extract the latest Boost Libraries
 * Create an environment variable called `BOOST_ROOT` which points to your Boost root directory (e.g. `C:\source\boost_1_63_0`)

#### x64 (Release & Debug)
 * Open the `VS2015 x64 Native Tools Command Prompt`
 * Navigate to your boost directory (e.g. `cd C:\source\boost_1_63_0`)
 * Run `bootstrap.bat`
 * Run `b2 toolset=msvc-14.0 link=static threading=multi runtime-link=static address-model=64 release stage` for release builds
   * Move the created `*.lib` files to `%BOOST_ROOT%\stage\lib\x64`
 * Run `b2 toolset=msvc-14.0 link=static threading=multi runtime-link=static address-model=64 debug stage` for debug builds
   * Move the created `*.lib` files to `%BOOST_ROOT%\stage\lib\x64`
 
#### x86 (Release & Debug)
 * Open the `VS2015 x86 Native Tools Command Prompt`
 * Navigate to your boost directory (e.g. `cd C:\source\boost_1_63_0`)
 * Run `bootstrap.bat`
 * Run `b2 toolset=msvc-14.0 link=static threading=multi runtime-link=static release stage` for release builds
   * Move the created `*.lib` files to `%BOOST_ROOT%\stage\lib\x86`
 * Run `b2 toolset=msvc-14.0 link=static threading=multi runtime-link=static address-model=64 debug stage` for debug builds
   * Move the created `*.lib` files to `%BOOST_ROOT%\stage\lib\x86`
 
