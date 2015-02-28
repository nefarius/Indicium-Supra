#pragma once

#define BUFSIZE	 4096
#define TIME_OUT 100

class CSerializer;

class CNamedPipeClient
{
public:
	CNamedPipeClient(CSerializer& bsIn, CSerializer& bsOut);

	bool Success() const;
private:
	bool m_bSuccess;
};