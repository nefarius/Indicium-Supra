#pragma once

#include <utils/windows.h>
#include <utils/NamedPipeClient.h>
#include <utils/BitStream.h>

#define EXPORT extern "C" __declspec(dllexport)

#define SERVER_CHECK(retn)	\
if (!IsServerAvailable())	\
{							\
	if (!API_Init())		\
		return retn;		\
	else					\
		Sleep(100);			\
}							\

bool IsServerAvailable();

EXPORT int  API_Init();
EXPORT void	SetParam(char *_szParamName, char *_szParamValue);