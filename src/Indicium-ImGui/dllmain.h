#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <MinHook.h>
#include "imgui.h"

#define INDICIUM_EXPORT extern "C" __declspec(dllexport) VOID __cdecl

template <typename T>
inline MH_STATUS MH_CreateHookApiEx(
	LPCWSTR pszModule, LPCSTR pszProcName, LPVOID pDetour, T** ppOriginal)
{
	return MH_CreateHookApi(
		pszModule, pszProcName, pDetour, reinterpret_cast<LPVOID*>(ppOriginal));
}

typedef LRESULT(WINAPI *tDefWindowProc)(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	);

LRESULT WINAPI DetourDefWindowProc(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	);

void HookDefWindowProc();
int init();
void RenderScene();

IMGUI_API LRESULT   ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
IMGUI_API LRESULT   ImGui_ImplDX10_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
IMGUI_API LRESULT   ImGui_ImplDX11_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
