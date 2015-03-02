#pragma once

#include <Utils/Windows.h>
#include <Utils/PipeClient.h>
#include <Utils/Serializer.h>

#define EXPORT extern "C" __declspec(dllexport)

#define SERVER_CHECK(retn)	\
if (!IsServerAvailable())	\
{							\
	if (!Init())			\
		return retn;		\
	else					\
		Sleep(250);			\
}							\

bool IsServerAvailable();

EXPORT int  Init();
EXPORT void	SetParam(char *_szParamName, char *_szParamValue);