#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define INDICIUM_EXPORT extern "C" __declspec(dllexport) VOID __cdecl

INDICIUM_EXPORT Reset(IID guid, LPVOID unknown) {}

INDICIUM_EXPORT EndScene(IID guid, LPVOID unknown) {}

INDICIUM_EXPORT ResizeTarget(IID guid, LPVOID unknown) {}

INDICIUM_EXPORT Present(IID guid, LPVOID unknown);
