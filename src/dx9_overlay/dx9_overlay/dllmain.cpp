#pragma comment(lib, "detours.lib")
#pragma comment(lib,"d3dx9.lib")

#include <utils/windows.h>
#include <game/game.h>

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	if (dwReason != DLL_PROCESS_ATTACH)
		return FALSE;

	if (GetModuleHandle("d3d9.dll"))
		return CreateThread(0, 0, (LPTHREAD_START_ROUTINE) initGame, 0, 0, 0) > 0;

	return FALSE;
}