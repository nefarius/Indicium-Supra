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
			BOOST_LOG_TRIVIAL(info) << "Shutting down hooks...";
			
			if(MH_DisableHook(MH_ALL_HOOKS) != MH_OK)
			{
				BOOST_LOG_TRIVIAL(error) << "Couldn't disable hooks, host process might crash";
			}
			else
			{
				BOOST_LOG_TRIVIAL(info) << "Hooks disabled";
			}
			
			if(MH_Uninitialize() != MH_OK)
			{
				BOOST_LOG_TRIVIAL(error) << "Couldn't shut down hook engine, host process might crash";
			}
			else
			{
				BOOST_LOG_TRIVIAL(info) << "Hook engine disabled";
			}
		}
		break;
	};

	return TRUE;
}

