#pragma once
#include <string>
#include <intarch.h>
#include <Utils/Windows.h>


void initGame();

void HookDX9(UINTX* vtable9);
void HookDX9Ex(UINTX* vtable9Ex);
void HookDX10(UINTX* vtable10SwapChain);
void HookDX11(UINTX* vtable11SwapChain);
void HookDX12(UINTX* vtable11SwapChain);
void HookDInput8(UINTX* vtable8);

