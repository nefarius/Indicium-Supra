#pragma once

#define BUFSIZE	 4096
#define TIME_OUT 100

class bitstream;

class namedpipeclient
{
public:
	namedpipeclient(const char *szPipe, bitstream *bsIn, bitstream *bsOut);

	bool Success() const;
private:
	bool m_bSuccess;
};