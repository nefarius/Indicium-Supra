# DX9-Overlay-API (LGPL v3)

An overlay API for DirectX 9 based games, which is licensed and distributed under the terms of the LGPL v3

## More information

http://www.overlay-api.net/

## How to build
1. Create an environment variable called `BOOST_ROOT` which points to your Boost root directory (e.g. `C:\Development\C++\boost_1_59_0`)
2. Create an environment variable called `DXSDK` which points to your DirectX SDK installation path (e.g. `C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)`)
3. MinHook should be downloaded by Visual Studio automatically, if not use NuGet Shell: `PM> Install-Package minhook`

### Prerequisites
* [Microsoft DirectX SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
* [Boost libraries](http://www.boost.org/)
* [MinHook](https://github.com/TsudaKageyu/minhook)
