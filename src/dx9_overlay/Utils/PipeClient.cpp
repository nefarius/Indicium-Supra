#include "Windows.h"
#include "PipeClient.h"
#include "Bitstream.h"

#include <Shared/Config.h>

#include <iostream>

PipeClient::PipeClient(Serializer& serializerIn, Serializer& serializerOut) :
m_bSuccess(false)
{
	char szData[BUFSIZE] = { 0 };
	char szPipe[MAX_PATH + 1] = { 0 };
	DWORD dwReaded;

	sprintf(szPipe, "\\\\.\\pipe\\%s", g_strPipeName);

	if (CallNamedPipe(szPipe, (LPVOID)serializerIn.GetData(), serializerIn.GetNumberOfBytesUsed(), szData, sizeof(szData), &dwReaded, TIME_OUT))
	{
		serializerOut.SetData(szData, sizeof(szData));
		m_bSuccess = true;
	}
}

bool PipeClient::success() const
{
	return m_bSuccess;
}