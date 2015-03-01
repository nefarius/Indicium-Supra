#include "Client.h"

#include <dllmain.h>
#include <ShlObj.h>

#include <Shared/PipeMessages.h>
#include <Utils/Misc.h>

#include <string>

#include <boost/algorithm/string.hpp>

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

	GetModuleFileName((HMODULE) g_hDllHandle, szDLLPath, sizeof(szDLLPath));
	if (!atoi(GetParam("use_window").c_str()))
	{
		std::string szSearchName = GetParam("process");
		dwPId = procIdByProcName(szSearchName);
	}
	else
	{
		std::string szSearchName = GetParam("window");
		dwPId = procIdByWindowName(szSearchName);
	}

	if (dwPId == 0)
		return 0;

	HANDLE hHandle = OpenProcess((STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0xFFF), FALSE, dwPId);
	if (hHandle == 0 || hHandle == INVALID_HANDLE_VALUE)
		return 0;

	void *pAddr = VirtualAllocEx(hHandle, NULL, strlen(szDLLPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (pAddr == NULL)
	{
		CloseHandle(hHandle);
		return 0;
	}

	bRetn = WriteProcessMemory(hHandle, pAddr, szDLLPath, strlen(szDLLPath) + 1, NULL);
	if (bRetn == FALSE)
	{
		VirtualFreeEx(hHandle, pAddr, strlen(szDLLPath) + 1, MEM_RELEASE);
		CloseHandle(hHandle);
		return 0;
	}

	DWORD dwBase = 0;

	// Load DLL-file
	{
		LPTHREAD_START_ROUTINE pFunc = (LPTHREAD_START_ROUTINE) GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
		if (pFunc == NULL)
		{
			VirtualFreeEx(hHandle, pAddr, strlen(szDLLPath) + 1, MEM_RELEASE);
			CloseHandle(hHandle);
			return 0;
		}

		HANDLE hThread = CreateRemoteThread(hHandle, 0, 0, pFunc, pAddr, 0, 0);
		if (hThread == NULL || hThread == INVALID_HANDLE_VALUE)
		{
			VirtualFreeEx(hHandle, pAddr, strlen(szDLLPath) + 1, MEM_RELEASE);
			CloseHandle(hHandle);
			return 0;
		}

		WaitForSingleObject(hThread, INFINITE);
		GetExitCodeThread(hThread, &dwBase);
		VirtualFreeEx(hHandle, pAddr, strlen(szDLLPath) + 1, MEM_RELEASE);
		CloseHandle(hThread);
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