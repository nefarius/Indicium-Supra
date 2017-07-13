#pragma once
#include <string>
#include <intarch.h>
#include <Utils/Windows.h>


// Boost includes
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>

void initGame();

void HookDX9(UINTX* vtable9);
void HookDX9Ex(UINTX* vtable9Ex);
void HookDX10(UINTX* vtable10SwapChain);
void HookDX11(UINTX* vtable11SwapChain);
void HookDInput8(UINTX* vtable8);

