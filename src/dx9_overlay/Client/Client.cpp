#include "Client.h"

#include <dllmain.h>
#include <ShlObj.h>

#include <Shared/PipeMessages.h>
#include <Utils/Misc.h>

#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/log/trivial.hpp>

struct stParamInfo
{
	std::string szParamName;
	std::string szParamValue;
};

stParamInfo g_paramArray[3] =
{
	"process", "",
	"window", "",
	"use_window", "0"
};

bool IsServerAvailable()
{
	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::Ping;

	return PipeClient(serializerIn, serializerOut).success();
}

EXPORT void SetParam(char *_szParamName, char *_szParamValue)
{
	for (int i = 0; i < ARRAYSIZE(g_paramArray); i++)
	if (boost::iequals(_szParamName, g_paramArray[i].szParamName))
		g_paramArray[i].szParamValue = _szParamValue;

	return;
}

std::string GetParam(char *_szParamName)
{
	for (int i = 0; i < ARRAYSIZE(g_paramArray); i++)
	if (boost::iequals(_szParamName, g_paramArray[i].szParamName))
		return g_paramArray[i].szParamValue;

	return "";
}


EXPORT int Init()
{
	char szDLLPath[MAX_PATH + 1] = { 0 };
	DWORD dwPId = 0;
	BOOL bRetn;

	BOOST_LOG_TRIVIAL(info) << "Initializing DLL injection";

	GetModuleFileName((HMODULE) g_hDllHandle, szDLLPath, sizeof(szDLLPath));
	if (!atoi(GetParam("use_window").c_str()))
	{
		std::string szSearchName = GetParam("process");
		BOOST_LOG_TRIVIAL(info) << "Searching for target process by process name: " << szSearchName;
		dwPId = procIdByProcName(szSearchName);
	}
	else
	{
		std::string szSearchName = GetParam("window");
		BOOST_LOG_TRIVIAL(info) << "Searching for target process by window name: " << szSearchName;
		dwPId = procIdByWindowName(szSearchName);
	}

	if (dwPId == 0)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Target PID not found";
		return 0;
	}

	BOOST_LOG_TRIVIAL(info) << "Target PID: " << dwPId;

	HANDLE hHandle = OpenProcess((STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0xFFF), FALSE, dwPId);
	if (hHandle == 0 || hHandle == INVALID_HANDLE_VALUE)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Couldn't open target process";
		return 0;
	}

	BOOST_LOG_TRIVIAL(info) << "Opened target process [HANDLE: " << hHandle << "]";

	void *pAddr = VirtualAllocEx(hHandle, NULL, strlen(szDLLPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (pAddr == NULL)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Couldn't allocate memory in target process";
		CloseHandle(hHandle);
		return 0;
	}

	BOOST_LOG_TRIVIAL(info) << "Allocated memory [ADDR: " << pAddr << "]";

	bRetn = WriteProcessMemory(hHandle, pAddr, szDLLPath, strlen(szDLLPath) + 1, NULL);
	if (bRetn == FALSE)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Couldn't write in memory region, performing clean-up";
		VirtualFreeEx(hHandle, pAddr, strlen(szDLLPath) + 1, MEM_RELEASE);
		CloseHandle(hHandle);
		return 0;
	}

	BOOST_LOG_TRIVIAL(info) << "Wrote DLL path (" << szDLLPath << ") to target process memory";

	DWORD dwBase = 0;

	// Load DLL-file
	{
		LPTHREAD_START_ROUTINE pFunc = (LPTHREAD_START_ROUTINE) GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
		if (pFunc == NULL)
		{
			BOOST_LOG_TRIVIAL(fatal) << "Couldn't find LoadLibraryA function or kernel32 module";
			VirtualFreeEx(hHandle, pAddr, strlen(szDLLPath) + 1, MEM_RELEASE);
			CloseHandle(hHandle);
			return 0;
		}

		HANDLE hThread = CreateRemoteThread(hHandle, 0, 0, pFunc, pAddr, 0, 0);
		if (hThread == NULL || hThread == INVALID_HANDLE_VALUE)
		{
			BOOST_LOG_TRIVIAL(fatal) << "Couldn't create remote thread";
			VirtualFreeEx(hHandle, pAddr, strlen(szDLLPath) + 1, MEM_RELEASE);
			CloseHandle(hHandle);
			return 0;
		}

		BOOST_LOG_TRIVIAL(info) << "Calling LoadLibraryA(" << szDLLPath << ") in target process";

		WaitForSingleObject(hThread, INFINITE);
		GetExitCodeThread(hThread, &dwBase);
		VirtualFreeEx(hHandle, pAddr, strlen(szDLLPath) + 1, MEM_RELEASE);
		CloseHandle(hThread);

		BOOST_LOG_TRIVIAL(info) << "Library loaded successfully";
	}
	
	// Start Remote-IPC
	{
		if (dwBase == 0)
		{
			CloseHandle(hHandle);
			return 0;
		}
			

		DWORD pFunc = (DWORD) GetProcAddress((HMODULE) g_hDllHandle, "enable");
		pFunc -= (DWORD) g_hDllHandle;
		pFunc += (DWORD) dwBase;

		if (pFunc == NULL)
		{
			CloseHandle(hHandle);
			return 0;
		}

		HANDLE hThread = CreateRemoteThread(hHandle, 0, 0, (LPTHREAD_START_ROUTINE) pFunc, 0, 0, 0);
		if (hThread == NULL || hThread == INVALID_HANDLE_VALUE)
		{
			CloseHandle(hHandle);
			return 0;
		}

		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
		CloseHandle(hHandle);

		return 1;
	}
}