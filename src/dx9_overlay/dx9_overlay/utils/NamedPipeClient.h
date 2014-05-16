#pragma once

#define BUFSIZE	 4096
#define TIME_OUT 100

class BitStream;

class CNamedPipeClient
{
public:
	CNamedPipeClient(const char *szPipe, BitStream *bsIn, BitStream *bsOut);

	bool Success() const;
private:
	bool m_bSuccess;
};