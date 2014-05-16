#pragma once
#include "windows.h"

#include <boost/function.hpp>
#include <boost/bind.hpp>

#define MAX_CLIENTS		16
#define BUFSIZE			4096
#define PIPE_TIMEOUT	5000

namespace boost { class thread; }
class BitStream;

class CNamedPipeServer
{
public:
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

	CNamedPipeServer(const char *pipe, boost::function<void(CNamedPipeServer::LPPIPEINSTANCE, BitStream *, BitStream *)> func);
	~CNamedPipeServer();

private:
	void Thread();
	BOOL ConnectToNewClient(HANDLE hPipe, LPOVERLAPPED lpo);
	void DisconnectAndReconnect(DWORD dwIdx);

	PIPEINSTANCE	m_Pipes[MAX_CLIENTS];
	HANDLE			m_hEvents[MAX_CLIENTS];

	char			m_szPipe[MAX_PATH];

	boost::thread	*m_thread;
	boost::function<void(CNamedPipeServer::LPPIPEINSTANCE, BitStream *, BitStream *)> m_cbCallback;
};

