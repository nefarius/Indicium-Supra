#pragma once

#define BUFSIZE	 4096
#define TIME_OUT 100

class CBitStream;

class CNamedPipeClient
{
public:
	CNamedPipeClient(const char *szPipe, CBitStream& bsIn, CBitStream& bsOut);

	bool Success() const;
private:
	bool m_bSuccess;
};