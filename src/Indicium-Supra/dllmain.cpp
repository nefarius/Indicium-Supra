#include "dllmain.h"

#include <Utils/Windows.h>
#include <Game/Game.h>
#include <boost/log/support/date_time.hpp>

HANDLE g_hDllHandle = nullptr;

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	g_hDllHandle = hInstance;

	DisableThreadLibraryCalls(static_cast<HMODULE>(hInstance));

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
			logging::add_common_attributes();

			logging::add_file_log
				(
					keywords::file_name = "Indicium-Supra.log",
					keywords::auto_flush = true,
					keywords::format = (
						expr::stream
						<< expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%d.%m.%Y - %H:%M:%S")
						<< ": [" << logging::trivial::severity
						<< "] " << expr::smessage
					)
				);

			logging::core::get()->set_filter
				(
					logging::trivial::severity >= logging::trivial::info
				);

			BOOST_LOG_TRIVIAL(info) << "Library loaded, attempting to launch main thread";

			auto hMain = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(initGame), nullptr, 0, nullptr);

			if (hMain == nullptr)
			{
				BOOST_LOG_TRIVIAL(error) << "Couldn't create main thread, library unusable";
				return FALSE;
			}

			BOOST_LOG_TRIVIAL(info) << "Main thread created successfully";
			return TRUE;
		}
	case DLL_PROCESS_DETACH:
		{
			BOOST_LOG_TRIVIAL(info) << "Shutting down hooks...";

			if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK)
			{
				BOOST_LOG_TRIVIAL(error) << "Couldn't disable hooks, host process might crash";
			}
			else
			{
				BOOST_LOG_TRIVIAL(info) << "Hooks disabled";
			}

			if (MH_Uninitialize() != MH_OK)
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

