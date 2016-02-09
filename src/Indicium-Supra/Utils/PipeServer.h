#pragma once
#include "Windows.h"

#include <boost/function.hpp>
#include <boost/bind.hpp>

#define MAX_CLIENTS		16
#define BUFSIZE			4096
#define PIPE_TIMEOUT	5000

namespace boost { class thread; }
class Serializer;

class PipeServer
{
	typedef struct
	{
		OVERLAPPED	m_Overlapped;
		HANDLE		m_hPipe;
		char		m_szRequest[BUFSIZE],
					m_szReply[BUFSIZE];
		DWORD		m_dwRead,
					m_dwToWrite,
					m_dwState;
		BOOL		m_fPendingIO;
	} PIPEINSTANCE, *LPPIPEINSTANCE;

public:
	PipeServer(boost::function<void(Serializer&, Serializer&)> func);
	~PipeServer();

private:
	void thread();
	BOOL connectToNewClient(HANDLE hPipe, LPOVERLAPPED lpo);
	void disconnectAndReconnect(DWORD dwIdx);

	PIPEINSTANCE m_Pipes[MAX_CLIENTS];
	HANDLE m_hEvents[MAX_CLIENTS];

	char m_szPipe[MAX_PATH];

	boost::thread *m_thread;
	boost::function<void(Serializer&, Serializer&)> m_cbCallback;
};

