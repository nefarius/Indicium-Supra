# Indicium-Supra (LGPL v3)

An overlay API for DirectX based games, which is licensed and distributed under the terms of the LGPL v3

## Prerequisites
* [Microsoft DirectX SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
* [Boost libraries](http://www.boost.org/)
* [MinHook](https://github.com/TsudaKageyu/minhook)

## How to build
### Build Boost Libraries
1. Download and extract the latest Boost Libraries
2. Create an environment variable called `BOOST_ROOT` which points to your Boost root directory (e.g. `C:\Development\C++\boost_1_60_0`)
3. Run `bootstrap.bat`
4. Fire up a command prompt and enter
 * `b2 toolset=msvc-12.0 link=static threading=multi runtime-link=static release stage` for 32-Bit release builds
   * Move the created `*.lib` files to `%BOOST_ROOT%\stage\lib\x86`
 * `b2 toolset=msvc-12.0 link=static threading=multi runtime-link=static debug stage` for 32-Bit debug builds
   * Move the created `*.lib` files to `%BOOST_ROOT%\stage\lib\x86`
 * `b2 toolset=msvc-12.0 link=static threading=multi runtime-link=static address-model=64 release stage` for 64-Bit release builds
   * Move the created `*.lib` files to `%BOOST_ROOT%\stage\lib\x64`
 * `b2 toolset=msvc-12.0 link=static threading=multi runtime-link=static address-model=64 debug stage` for 64-Bit debug builds
   * Move the created `*.lib` files to `%BOOST_ROOT%\stage\lib\x64`

