#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include "imgui.h"

typedef LRESULT(WINAPI *t_WindowProc)(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	);

LRESULT WINAPI DetourWindowProc(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	);

void HookWindowProc(HWND hWnd);
int init();
void RenderScene();

extern IMGUI_API LRESULT   ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern IMGUI_API LRESULT   ImGui_ImplDX10_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern IMGUI_API LRESULT   ImGui_ImplDX11_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
