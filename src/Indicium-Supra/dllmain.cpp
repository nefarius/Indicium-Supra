#include "dllmain.h"

#include <Utils/Windows.h>
#include <Game/Game.h>

HANDLE g_hDllHandle = nullptr;

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	g_hDllHandle = hInstance;

	DisableThreadLibraryCalls(static_cast<HMODULE>(hInstance));

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		return CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(initGame), nullptr, 0, nullptr) > nullptr;
	case DLL_PROCESS_DETACH:
		{
			MH_DisableHook(MH_ALL_HOOKS);
			MH_Uninitialize();
		}
		break;
	};

	return TRUE;
}

