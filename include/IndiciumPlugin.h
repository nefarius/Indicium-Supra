#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

typedef enum _Direct3DVersion : char
{
    Direct3D9 = 0,
    Direct3D9Ex = 1 << 0,
    Direct3D10 = 1 << 1,
    Direct3D11 = 1 << 2

} Direct3DVersion;

#ifndef IS_INDICIUM_SUPRA

#define INDICIUM_EXPORT extern "C" __declspec(dllexport) VOID __cdecl

INDICIUM_EXPORT Present(IID guid, LPVOID unknown, Direct3DVersion version);

INDICIUM_EXPORT Reset(IID guid, LPVOID unknown, Direct3DVersion version) {}

INDICIUM_EXPORT EndScene(IID guid, LPVOID unknown, Direct3DVersion version) {}

INDICIUM_EXPORT ResizeTarget(IID guid, LPVOID unknown, Direct3DVersion version) {}

#endif
