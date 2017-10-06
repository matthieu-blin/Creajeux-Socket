#include "WinSocket.h"

bool OS_Init()
{
	WSAData data;
	if( WSAStartup( MAKEWORD( 2, 2 ), &data ) != 0)
	{
		WSACleanup();
		return false;
	}
	return true;
}

void OS_End()
{
	WSACleanup();
}

OS_Sockfd OS_Socket(int af, int type, int proto)
{

	return socket(af, type, proto);
}

int OS_Bind(OS_Sockfd s, const sockaddr *addr, int addrlen)
{
	return bind(s, addr, addrlen);
}

int OS_Listen(OS_Sockfd s, int backlog)
{
	return listen(s, backlog);
}

int OS_Connect(OS_Sockfd s, const sockaddr *addr, int addrlen)
{
	return connect(s, addr, addrlen);
}

OS_Sockfd OS_Accept(OS_Sockfd s, sockaddr *addr, int *addrlen)
{
	return accept(s, addr, addrlen);
}

int OS_CloseSocket(OS_Sockfd s)
{
	return closesocket(s);
}

int OS_Send(OS_Sockfd s, const char *buf, size_t len, int flags)
{
	return send(s, buf, len, flags);
}

int OS_SendTo(OS_Sockfd s, const char *buf, size_t len, int flags, const sockaddr *to, int tolen)
{
	return sendto(s, buf, len, flags, to, tolen);
}

int OS_Recv(OS_Sockfd s, char *buf, size_t len, int flags)
{
	return recv(s, buf, len, flags);
}

int OS_RecvFrom(OS_Sockfd s, char *buf, int len, int flags, sockaddr *from, int *fromlen)
{
	return recvfrom(s, buf, len, flags, from, fromlen);
}

int OS_Setsockopt(OS_Sockfd s, int level, int optname, const char *optval, int optlen)
{
	return setsockopt(s, level, optname, optval, optlen);
}

int OS_Getsockopt(OS_Sockfd s, int level, int optname, char *optval, int *optlen)
{
	return getsockopt(s, level, optname, optval, optlen);
}

int OS_Select(int nfds, fd_set *rd, fd_set *wr, fd_set *ex, struct timeval *timeout)
{
	return select(nfds, rd, wr, ex, timeout);
}

//////////////////////////////////////////////////////////////////////////
int OS_SetNonBlocking(OS_Sockfd s)
{
	unsigned long mode = 1;
	int ret = ioctlsocket(s, FIONBIO, &mode);

	if (ret == SOCKET_ERROR)
		return -1;

	return 0;
}
//////////////////////////////////////////////////////////////////////////
int OS_SetBlocking(OS_Sockfd s)
{
	u_long value = 0;
	if (ioctlsocket(s, FIONBIO, &value) == SOCKET_ERROR)
	{
		return -1;
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
int OS_SetDFFlag(OS_Sockfd s)
{
	int val = 1;
	return setsockopt(s, IPPROTO_IP, IP_DONTFRAGMENT, (const char*)&val, sizeof(val));
}

unsigned long OS_InetAddr(const char *cp)
{
	return inet_addr(cp);
}
//////////////////////////////////////////////////////////////////////////
const char* OS_InetNtoP(int family, const void * pSrcAddress, char* pDstAddress, socklen_t size)
{
	return inet_ntop(family, const_cast<void *>(pSrcAddress), pDstAddress, size);
}
//////////////////////////////////////////////////////////////////////////
int OS_InetPtoN(int family, const char * pSrcAddress, void * pDstAddress)
{
	return inet_pton(family, pSrcAddress, pDstAddress);
}
//////////////////////////////////////////////////////////////////////////
hostent* OS_GetHostByName(const char *name)
{
	return gethostbyname(name);
}

int OS_ShutdownSocket(OS_Sockfd s, int how)
{
	return shutdown(s, how);
}

int OS_SetSendBufSize(OS_Sockfd s, int sendBufSize)
{
	// Set send buf size
	OS_Setsockopt(s, SOL_SOCKET, SO_SNDBUF, (const char *)&sendBufSize, sizeof(int));
	// Get send buf size
	int varSize = sizeof(int);
	OS_Getsockopt(s, SOL_SOCKET, SO_SNDBUF, (char *)&sendBufSize, &varSize);

	return sendBufSize;
}

int OS_SetRecvBufSize(OS_Sockfd s, int recvBufSize)
{
	// Set recv buf size
	OS_Setsockopt(s, SOL_SOCKET, SO_RCVBUF, (const char *)&recvBufSize, sizeof(int));
	// Get recv buf size
	int varSize = sizeof(int);
	OS_Getsockopt(s, SOL_SOCKET, SO_RCVBUF, (char *)&recvBufSize, &varSize);

	return recvBufSize;
}

int OS_GetSockName(OS_Sockfd s, sockaddr * pName, socklen_t * pNamelen)
{
	return getsockname(s, pName, pNamelen);
}