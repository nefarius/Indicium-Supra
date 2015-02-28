#include "Windows.h"
#include "PipeClient.h"
#include "Bitstream.h"

#include <Shared/Config.h>

#include <iostream>

PipeClient::PipeClient(Serializer& bsIn, Serializer& bsOut) :
m_bSuccess(false)
{
	char szData[BUFSIZE] = { 0 };
	char szPipe[MAX_PATH + 1] = { 0 };
	DWORD dwReaded;

	sprintf(szPipe, "\\\\.\\pipe\\%s", g_strPipeName);

	if (CallNamedPipe(szPipe, (LPVOID)bsIn.GetData(), bsIn.GetNumberOfBytesUsed(), szData, sizeof(szData), &dwReaded, TIME_OUT))
	{
		bsOut.SetData(szData, sizeof(szData));
		m_bSuccess = true;
	}
}

bool PipeClient::success() const
{
	return m_bSuccess;
}