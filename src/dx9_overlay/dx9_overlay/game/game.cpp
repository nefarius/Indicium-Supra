#include <utils/windows.h>
#include <utils/hook.h>
#include <utils/pattern.h>
#include "rendering/Renderer.h"

#include "game.h"

#include <d3dx9.h>

Hook<convention_type::stdcall_t, HRESULT, LPDIRECT3DDEVICE9, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *> g_presentHook;
Hook<convention_type::stdcall_t, HRESULT, LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS *> g_resetHook;

CRenderer g_renderer;

void initGame()
{
	HMODULE hMod = NULL;

	while ((hMod = GetModuleHandle("d3d9.dll")) == NULL)
		Sleep(200);

	DWORD *vtbl = 0;
	DWORD dwDevice = findPattern((DWORD) hMod, 0x128000, (PBYTE)"\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86", "xx????xx????xx");
	memcpy(&vtbl, (void *) (dwDevice + 0x2), 4);

	g_presentHook.apply(vtbl[17], [](LPDIRECT3DDEVICE9 dev, CONST RECT * a1, CONST RECT * a2, HWND a3, CONST RGNDATA *a4) -> HRESULT
	{
		__asm pushad
		g_renderer.Draw(dev);
		__asm popad

		return g_presentHook.callOrig(dev, a1, a2, a3, a4);
	});

	g_resetHook.apply(vtbl[16], [](LPDIRECT3DDEVICE9 dev, D3DPRESENT_PARAMETERS *pp) -> HRESULT
	{
		__asm pushad
		g_renderer.Reset(dev);
		__asm popad

		return g_resetHook.callOrig(dev, pp);
	});
}
