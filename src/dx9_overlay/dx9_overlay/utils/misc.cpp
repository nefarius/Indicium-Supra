#include "misc.h"
#include "windows.h"

#include <TlHelp32.h>
#include <boost/algorithm/string/case_conv.hpp>

unsigned long GetProcIdByWindowName(std::string windName)
{
	DWORD dwPID = 0;
	HWND wHandle = 0;
	wHandle = FindWindow(NULL, windName.c_str());
	if (wHandle == 0 || wHandle == INVALID_HANDLE_VALUE)
		return dwPID;

	GetWindowThreadProcessId(wHandle, &dwPID);
	return dwPID;
}

unsigned long GetProcIdByProcName(std::string procName)
{
	PROCESSENTRY32 entry;
	DWORD dwPID = 0;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == 0 || hSnapshot == INVALID_HANDLE_VALUE)
		return dwPID;

	entry.dwSize = sizeof(entry);
	Process32First(hSnapshot, &entry);
	while (Process32Next(hSnapshot, &entry))
	{
		std::string szExeFile = entry.szExeFile;
		boost::algorithm::to_lower(szExeFile);
		if (szExeFile.find(procName) != std::string::npos)
		{
			dwPID = entry.th32ProcessID;
			break;
		}
	}
	CloseHandle(hSnapshot);
	return dwPID;
}