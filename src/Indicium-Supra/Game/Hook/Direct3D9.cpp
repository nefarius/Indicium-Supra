/*
MIT License

Copyright (c) 2018 Benjamin Höglinger

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Direct3D9.h"
#include "Exceptions.hpp"

using namespace Indicium::Core::Exceptions;


Direct3D9Hooking::Direct3D9::Direct3D9() :
    d3d9(nullptr), d3d9_device(nullptr)
{
    temp_window = std::make_unique<Window>("TempDirect3D9OverlayWindow");

    const auto hMod = GetModuleHandle("d3d9.dll");

    if (hMod == nullptr)
    {
        throw ModuleNotFoundException("Could not get the handle to d3d9.dll");
    }

    const auto Direct3DCreate9 = static_cast<LPVOID>(GetProcAddress(hMod, "Direct3DCreate9"));
    if (Direct3DCreate9 == nullptr)
    {
        throw ProcAddressNotFoundException("Could not locate the Direct3DCreate9 procedure entry point");
    }

    d3d9 = static_cast<LPDIRECT3D9(WINAPI *)(UINT)>(Direct3DCreate9)(D3D_SDK_VERSION);
    if (d3d9 == nullptr)
    {
        throw RuntimeException("Could not create the DirectX 9 interface");
    }

    D3DDISPLAYMODE display_mode;
    const auto hr = d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display_mode);
    if (FAILED(hr))
    {
        throw DXAPIException("Could not determine the current display mode", hr);
    }

    D3DPRESENT_PARAMETERS present_parameters;
    ZeroMemory(&present_parameters, sizeof(D3DPRESENT_PARAMETERS));
    present_parameters.Windowed = TRUE;
    present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    present_parameters.BackBufferFormat = display_mode.Format;

    const auto error_code = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, temp_window->windowHandle(),
                                               D3DCREATE_SOFTWARE_VERTEXPROCESSING |
                                               D3DCREATE_DISABLE_DRIVER_MANAGEMENT, &present_parameters, &d3d9_device);
    if (FAILED(error_code))
    {
        throw DXAPIException("Could not create the Direct3D 9 device", error_code);
    }
}

std::vector<size_t> Direct3D9Hooking::Direct3D9::vtable() const
{
    size_t vtbl[VTableElements];
    memcpy(vtbl, *reinterpret_cast<size_t **>(d3d9_device), VTableElements * sizeof(size_t));
    return std::vector<size_t>(vtbl, vtbl + sizeof vtbl / sizeof vtbl[0]);
}

Direct3D9Hooking::Direct3D9::~Direct3D9()
{
    if (d3d9_device)
        d3d9_device->Release();

    if (d3d9)
        d3d9->Release();
}
