#include "PipeServer.h"
#include "Serializer.h"

#include <Shared/Config.h>

#include <boost/thread.hpp>

#define CONNECTING_STATE	0 
#define READING_STATE		1 
#define WRITING_STATE		2 

BOOL PipeServer::connectToNewClient(HANDLE hPipe, LPOVERLAPPED lpo)
{
	BOOL fConnected, fPendingIO = FALSE;

	fConnected = ConnectNamedPipe(hPipe, lpo);

	if (fConnected)
		return 0;

	switch (GetLastError())
	{
	case ERROR_IO_PENDING:
		fPendingIO = TRUE;
		break;

	case ERROR_PIPE_CONNECTED:
		if (SetEvent(lpo->hEvent))
			break;
	default:
	{
		return 0;
	}
	}

	return fPendingIO;
}

void PipeServer::disconnectAndReconnect(DWORD dwIdx)
{
	DisconnectNamedPipe(m_Pipes[dwIdx].m_hPipe);
	m_Pipes[dwIdx].m_fPendingIO = connectToNewClient(m_Pipes[dwIdx].m_hPipe, &m_Pipes[dwIdx].m_Overlapped);
	m_Pipes[dwIdx].m_dwState = m_Pipes[dwIdx].m_fPendingIO ? CONNECTING_STATE : READING_STATE;

}
PipeServer::PipeServer(boost::function<void(Serializer&, Serializer&)> func) : m_cbCallback(func), m_thread(0)
{
	memset(m_szPipe, 0, sizeof(m_szPipe));
	memset(m_Pipes, 0, sizeof(m_Pipes));

	sprintf_s(m_szPipe, "\\\\.\\pipe\\%s", g_strPipeName);

	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		m_hEvents[i] = CreateEvent(NULL, TRUE, TRUE, NULL);
		m_Pipes[i].m_Overlapped.hEvent = m_hEvents[i];
		m_Pipes[i].m_hPipe = CreateNamedPipeA(m_szPipe, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
			MAX_CLIENTS, BUFSIZE, BUFSIZE, PIPE_TIMEOUT, NULL);

		m_Pipes[i].m_fPendingIO = connectToNewClient(m_Pipes[i].m_hPipe, &m_Pipes[i].m_Overlapped);
		m_Pipes[i].m_dwState = m_Pipes[i].m_fPendingIO ? CONNECTING_STATE : READING_STATE;
	}

	m_thread = new boost::thread(boost::bind(&PipeServer::thread, this));
}


PipeServer::~PipeServer(void)
{
	if (m_thread)
	{
		m_thread->interrupt();
		if (m_thread->joinable())
			m_thread->join();
		delete m_thread;
		m_thread = NULL;
	}
}

void PipeServer::thread()
{
	BOOL bSuccess;
	DWORD dwRet;

	while (true)
	{
		boost::this_thread::interruption_point();

		DWORD dwWait = WaitForMultipleObjects(MAX_CLIENTS, m_hEvents, FALSE, INFINITE);

		int idx = dwWait - WAIT_OBJECT_0;
		if (idx < 0 || idx >(MAX_CLIENTS - 1))
			continue;

		if (m_Pipes[idx].m_fPendingIO)
		{
			bSuccess = GetOverlappedResult(m_Pipes[idx].m_hPipe, &m_Pipes[idx].m_Overlapped, &dwRet, FALSE);

			switch (m_Pipes[idx].m_dwState)
			{
			case CONNECTING_STATE:
				if (!bSuccess)
					return;
				m_Pipes[idx].m_dwState = READING_STATE;
				break;
			case READING_STATE:
				if (!bSuccess || dwRet == 0)
				{
					disconnectAndReconnect(idx);
					continue;
				}
				m_Pipes[idx].m_dwRead = dwRet;
				m_Pipes[idx].m_dwState = WRITING_STATE;
				break;
			case WRITING_STATE:
				if (!bSuccess || dwRet != m_Pipes[idx].m_dwToWrite)
				{
					disconnectAndReconnect(idx);
					continue;
				}
				m_Pipes[idx].m_dwState = READING_STATE;
				break;
			default:
				return;
			}
		}

		switch (m_Pipes[idx].m_dwState)
		{

		case READING_STATE:
			bSuccess = ReadFile(m_Pipes[idx].m_hPipe, m_Pipes[idx].m_szRequest, BUFSIZE, &m_Pipes[idx].m_dwRead, &m_Pipes[idx].m_Overlapped);

			if (bSuccess && m_Pipes[idx].m_dwRead != 0)
			{
				m_Pipes[idx].m_fPendingIO = FALSE;
				m_Pipes[idx].m_dwState = WRITING_STATE;
				continue;
			}
			if (!bSuccess && (GetLastError() == ERROR_IO_PENDING))
			{
				m_Pipes[idx].m_fPendingIO = TRUE;
				continue;
			}
			disconnectAndReconnect(idx);
			break;
		case WRITING_STATE:
			Serializer serializerIn(m_Pipes[idx].m_szRequest, m_Pipes[idx].m_dwRead);
			Serializer serializerOut;

			m_cbCallback(serializerIn, serializerOut);

			memset(m_Pipes[idx].m_szReply, 0, BUFSIZE);
			memset(m_Pipes[idx].m_szRequest, 0, BUFSIZE);
			memcpy(m_Pipes[idx].m_szReply, serializerOut.data(), serializerOut.numberOfBytesUsed());

			m_Pipes[idx].m_dwToWrite = serializerOut.numberOfBytesUsed();

			bSuccess = WriteFile(m_Pipes[idx].m_hPipe, m_Pipes[idx].m_szReply, m_Pipes[idx].m_dwToWrite, &dwRet, &m_Pipes[idx].m_Overlapped);
			if (bSuccess && dwRet == m_Pipes[idx].m_dwToWrite)
			{
				m_Pipes[idx].m_fPendingIO = FALSE;
				m_Pipes[idx].m_dwState = READING_STATE;
				continue;
			}
			if (!bSuccess && GetLastError() == ERROR_IO_PENDING)
			{
				m_Pipes[idx].m_fPendingIO = true;
				continue;
			}
			disconnectAndReconnect(idx);
			break;
		}
	}
}

