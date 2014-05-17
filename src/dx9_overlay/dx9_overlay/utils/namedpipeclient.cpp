#include "windows.h"
#include "namedpipeclient.h"
#include "bitstream.h"

#include <iostream>



namedpipeclient::namedpipeclient(const char *Pipe, bitstream *bsIn, bitstream *bsOut) :
m_bSuccess(false)
{
	char szData[BUFSIZE] = { 0 };
	char szPipe[MAX_PATH + 1] = { 0 };
	DWORD dwReaded;

	sprintf(szPipe, "\\\\.\\pipe\\%s", Pipe);

	if (CallNamedPipe(szPipe, (LPVOID)bsIn->GetData(), bsIn->GetNumberOfBytesUsed(), szData, sizeof(szData), &dwReaded, TIME_OUT))
	{
		bsOut->SetData(szData, sizeof(szData));
		m_bSuccess = true;
	}
}

bool namedpipeclient::Success() const
{
	return m_bSuccess;
}