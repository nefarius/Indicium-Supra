#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

typedef enum _Direct3DVersion : char
{
    Direct3D9 = 0,
    Direct3D9Ex = 1 << 0,
    Direct3D10 = 1 << 1,
    Direct3D11 = 1 << 2,
    Direct3D12 = 1 << 3

} Direct3DVersion;

#define IS_DIRECT3D9(_v_)       ((_v_ & Direct3DVersion::Direct3D9) != 0)
#define IS_DIRECT3D9EX(_v_)     ((_v_ & Direct3DVersion::Direct3D9Ex) != 0)
#define IS_DIRECT3D10(_v_)      ((_v_ & Direct3DVersion::Direct3D10) != 0)
#define IS_DIRECT3D11(_v_)      ((_v_ & Direct3DVersion::Direct3D11) != 0)
#define IS_DIRECT3D12(_v_)      ((_v_ & Direct3DVersion::Direct3D12) != 0)

#ifndef IS_INDICIUM_SUPRA

#define INDICIUM_EXPORT extern "C" __declspec(dllexport) VOID __cdecl

INDICIUM_EXPORT Present(IID guid, LPVOID unknown, Direct3DVersion version);

INDICIUM_EXPORT Reset(IID guid, LPVOID unknown, Direct3DVersion version);

INDICIUM_EXPORT EndScene(IID guid, LPVOID unknown, Direct3DVersion version);

INDICIUM_EXPORT ResizeTarget(IID guid, LPVOID unknown, Direct3DVersion version);

#endif
