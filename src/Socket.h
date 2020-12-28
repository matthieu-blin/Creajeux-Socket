#pragma once
#if  _WIN32 
#include "WinSocket.h"
#endif
//
//  Network
//
bool OS_Init();
void OS_End();
OS_Sockfd OS_Socket(int af, int type, int proto);
int OS_Bind(OS_Sockfd s, const sockaddr *addr, int addrlen);
int OS_Listen(OS_Sockfd s, int backlog);
OS_Sockfd OS_Accept(OS_Sockfd s, sockaddr *addr, int *addrlen);
int OS_Connect(OS_Sockfd, const sockaddr *addr, int addrlen);
int OS_CloseSocket(OS_Sockfd s);
int OS_ShutdownSocket(OS_Sockfd s, int how);
int OS_Send(OS_Sockfd s, const char *buf, size_t len, int flags);
int OS_SendTo(OS_Sockfd s, const char *buf, size_t len, int flags, const sockaddr *to, socklen_t tolen);
int OS_Recv(OS_Sockfd s, char *buf, size_t len, int flags);
int OS_RecvFrom(OS_Sockfd s, char *buf, size_t len, int flags, sockaddr *from, socklen_t *fromlen);
int OS_Setsockopt(OS_Sockfd s, int level, int optname, const char *optval, int optlen);
int OS_Getsockopt(OS_Sockfd s, int level, int optname, char *optval, int *optlen);
int OS_SocketAbort(OS_Sockfd s, int flags);
int OS_SetNonBlocking(OS_Sockfd s);
int OS_SetBlocking(OS_Sockfd s);
int OS_SetDFFlag(OS_Sockfd s); // not implemented for VITA, PS3, IOS
int OS_SetDFV6Flag(OS_Sockfd s); // not implemented for PS4, VITA, PS3, IOS
const char* OS_InetNtoP(int family, const char* pSrcAddress, char* pDstAddress, socklen_t size);
int OS_InetPtoN(int family, const char * pSrcAddress, char* pDst);
int OS_SetSendBufSize(OS_Sockfd s, int sendBufSize);
int OS_SetRecvBufSize(OS_Sockfd s, int recvBufSize);
int OS_GetSockName(OS_Sockfd s, sockaddr * pName, socklen_t * pNamelen);
hostent* OS_GetHostByName(const char *name); // only supported for PS platforms for IPv4, on other platforms use OS_GetNameInfo/OS_GetAddrInfo for IPv4 and IPv6
int OS_GetNameInfo(const struct sockaddr *sa, socklen_t salen, char *hostname, size_t hostlen, char *servicename, size_t servicelen, int flags);
int OS_GetAddrInfo(const char *hostname, const char *servicename, const struct addrinfo *hints, struct addrinfo **res);
int OS_Select(int nfds, fd_set *rd, fd_set *wr, fd_set *ex, struct timeval *timeout);


