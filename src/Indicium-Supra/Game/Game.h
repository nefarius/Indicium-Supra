#pragma once
#include <string>
#include <intarch.h>
#include <Utils/Windows.h>
#include <MinHook.h>
#include <boost/log/trivial.hpp>

extern class Renderer g_pRenderer;

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

void initGame();
void logOnce(std::string message);

void HookDefWindowProc();
void HookDX9(UINTX* vtable9);
void HookDX9Ex(UINTX* vtable9Ex);
void HookDX10(UINTX* vtable10SwapChain);
void HookDX11(UINTX* vtable11SwapChain);
void HookDInput8(UINTX* vtable8);

void RenderScene();