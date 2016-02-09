#pragma once

#define BUFSIZE	 4096
#define TIME_OUT 100

class Serializer;

class PipeClient
{
public:
	PipeClient(Serializer& serializerIn, Serializer& serializerOut);

	bool success() const;
private:
	bool m_bSuccess;
};