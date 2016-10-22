#pragma once

#include <list>
#include <vector>
#include <string>

#include "socket_helper.h"
#include "socket_io.h"

class xconnector_t;

struct XSocketManager : ISocketManager
{
	XSocketManager();
	~XSocketManager();

	bool Setup(int max_connection);

	ISocketListener* Listen(const char szIP[], int nPort) override;

	connector_t* connect(const char node[], const char service[]) override;

	void Wait(int nTimeout = 16) override;
	void Release() override { delete this; }

	ISocketStream* CreateStreamSocket(socket_t nSocket, size_t uRecvBufferSize, size_t uSendBufferSize, const std::string& strRemoteIP);
	void ProcessSocketEvent(int nTimeout);

#ifdef _MSC_VER
	HANDLE m_hCompletionPort = INVALID_HANDLE_VALUE;
	std::vector<OVERLAPPED_ENTRY> m_Events;
#endif

#ifdef __linux
	int m_nEpoll = -1;
	std::vector<epoll_event> m_Events;
#endif

#ifdef __APPLE__
	int m_nKQ = -1;
	std::vector<struct kevent> m_Events;
#endif

	char m_szError[64];
	int m_max_connection = 0;

	std::list<xconnector_t*> m_connectors;
	std::list<struct XSocketStream*> m_streams;
	std::list<struct XSocketListener*> m_listeners;
};
