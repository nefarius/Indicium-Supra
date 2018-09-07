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

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "imgui.h"

#include <Indicium/Engine/IndiciumCore.h>
#include <Indicium/Engine/IndiciumDirect3D9.h>
#include <Indicium/Engine/IndiciumDirect3D10.h>
#include <Indicium/Engine/IndiciumDirect3D11.h>

typedef LRESULT(WINAPI *t_WindowProc)(
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

LRESULT WINAPI DetourWindowProc(
    _In_ HWND hWnd,
    _In_ UINT Msg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
);

void HookWindowProc(HWND hWnd);
void RenderScene();

bool ImGui_ImplWin32_UpdateMouseCursor();
IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

EVT_INDICIUM_GAME_HOOKED EvtIndiciumGameHooked;
EVT_INDICIUM_GAME_UNHOOKED EvtIndiciumGameUnhooked;

EVT_INDICIUM_D3D9_PRESENT EvtIndiciumD3D9Present;
EVT_INDICIUM_D3D9_RESET EvtIndiciumD3D9PreReset;
EVT_INDICIUM_D3D9_RESET EvtIndiciumD3D9PostReset;
EVT_INDICIUM_D3D9_PRESENT_EX EvtIndiciumD3D9PresentEx;
EVT_INDICIUM_D3D9_RESET_EX EvtIndiciumD3D9PreResetEx;
EVT_INDICIUM_D3D9_RESET_EX EvtIndiciumD3D9PostResetEx;

EVT_INDICIUM_D3D10_PRESENT EvtIndiciumD3D10Present;
EVT_INDICIUM_D3D10_RESIZE_BUFFERS EvtIndiciumD3D10PreResizeBuffers;
EVT_INDICIUM_D3D10_RESIZE_BUFFERS EvtIndiciumD3D10PostResizeBuffers;

EVT_INDICIUM_D3D11_PRESENT EvtIndiciumD3D11Present;
EVT_INDICIUM_D3D11_RESIZE_BUFFERS EvtIndiciumD3D11PreResizeBuffers;
EVT_INDICIUM_D3D11_RESIZE_BUFFERS EvtIndiciumD3D11PostResizeBuffers;


/**
 * \fn	TOGGLE_STATE(int key, bool& toggle)
 *
 * \brief	Overly complicated key toggle helper
 *
 * \author	Benjamin "Nefarius" Höglinger-Stelzer
 * \date	7/09/2018
 *
 * \param 		  	key   	The key.
 * \param [in,out]	toggle	The value to toggle.
 */
VOID
FORCEINLINE
TOGGLE_STATE(int key, bool& toggle)
{
	static auto pressedPast = false, pressedNow = false;

	if (GetAsyncKeyState(key) & 0x8000)
	{
		pressedNow = true;
	}
	else
	{
		pressedPast = false;
		pressedNow = false;
	}

	if (!pressedPast && pressedNow)
	{
		toggle = !toggle;

		pressedPast = true;
	}
}
