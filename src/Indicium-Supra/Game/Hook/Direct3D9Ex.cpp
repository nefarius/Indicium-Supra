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

#include "Direct3D9Ex.h"
#include "Exceptions.hpp"

using namespace Indicium::Core::Exceptions;

Direct3D9Hooking::Direct3D9Ex::Direct3D9Ex() :
    d3d9_ex(nullptr), d3d9_device_ex(nullptr)
{
    temp_window = std::make_unique<Window>("TempDirect3D9ExOverlayWindow");

    const auto hMod = GetModuleHandle("d3d9.dll");

    if (hMod == nullptr)
    {
        throw ModuleNotFoundException("Could not get the handle to d3d9.dll");
    }

    const auto Direct3DCreate9Ex = static_cast<LPVOID>(GetProcAddress(hMod, "Direct3DCreate9Ex"));
    if (Direct3DCreate9Ex == nullptr)
    {
        throw RuntimeException("Could not locate the Direct3DCreate9 procedure entry point");
    }

    auto error_code = static_cast<HRESULT(WINAPI *)(UINT, IDirect3D9Ex**)>(Direct3DCreate9Ex
    )(D3D_SDK_VERSION, &d3d9_ex);
    if (FAILED(error_code))
    {
        throw DXAPIException("Could not create the DirectX 9 interface", error_code);
    }

    D3DDISPLAYMODE display_mode;
    error_code = d3d9_ex->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display_mode);
    if (FAILED(error_code))
    {
        throw DXAPIException("Could not determine the current display mode", error_code);
    }

    D3DPRESENT_PARAMETERS present_parameters;
    ZeroMemory(&present_parameters, sizeof(D3DPRESENT_PARAMETERS));
    present_parameters.Windowed = TRUE;
    present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    present_parameters.BackBufferFormat = display_mode.Format;

    error_code = d3d9_ex->CreateDeviceEx(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        temp_window->windowHandle(),
        D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
        &present_parameters,
        nullptr,
        &d3d9_device_ex
    );
    if (FAILED(error_code))
    {
        throw DXAPIException("Could not create the Direct3D 9 device", error_code);
    }
}

std::vector<size_t> Direct3D9Hooking::Direct3D9Ex::vtable() const
{
    size_t vtbl[VTableElements];
    memcpy(vtbl, *reinterpret_cast<size_t **>(d3d9_device_ex), VTableElements * sizeof(size_t));
    return std::vector<size_t>(vtbl, vtbl + sizeof vtbl / sizeof vtbl[0]);
}

Direct3D9Hooking::Direct3D9Ex::~Direct3D9Ex()
{
    if (d3d9_device_ex)
        d3d9_device_ex->Release();

    if (d3d9_ex)
        d3d9_ex->Release();
}
