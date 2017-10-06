#pragma once
#include <stdio.h>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "Ws2_32.lib")
//
//  Network
//
// Socket type
#define OS_IPPROTO_IP       IPPROTO_IP  
#define OS_IPPROTO_ICMP     IPPROTO_ICMP
#define OS_IPPROTO_IGMP     IPPROTO_IGMP
#define OS_IPPROTO_TCP      IPPROTO_TCP 
#define OS_IPPROTO_UDP      IPPROTO_UDP 

// Network types
typedef SOCKET              OS_Sockfd;
#if !defined (__cplusplus)
typedef struct sockaddr     sockaddr;
#endif

// Socket options
// IP
#define OS_IP_HDRINCL           IP_HDRINCL
#define OS_IP_TOS               IP_TOS
#define OS_IP_TTL               IP_TTL
#define OS_IP_MULTICAST_IF      IP_MULTICAST_IF
#define OS_IP_MULTICAST_TTL     IP_MULTICAST_TTL
#define OS_IP_MULTICAST_LOOP    IP_MULTICAST_LOOP
#define OS_IP_ADD_MEMBERSHIP    IP_ADD_MEMBERSHIP
#define OS_IP_DROP_MEMBERSHIP   IP_DROP_MEMBERSHIP
#define OS_IP_TTLCHK            IP_TTLCHK
#define OS_IP_MAXTTL            IP_MAXTTL
// TCP
#define OS_TCP_NODELAY          TCP_NODELAY
#define OS_TCP_MAXSEG           TCP_MAXSEG
// SOCKET
#define OS_SOL_SOCKET       SOL_SOCKET
#define OS_SO_REUSEADDR     SO_REUSEADDR
#define OS_SO_KEEPALIVE     SO_KEEPALIVE
#define OS_SO_BROADCAST     SO_BROADCAST
#define OS_SO_LINGER        SO_LINGER
#define OS_SO_OOBINLINE     SO_OOBINLINE
#define OS_SO_REUSEPORT     SO_REUSEPORT
#define OS_SO_ONESBCAST     SO_ONESBCAST
#define OS_SO_USECRYPTO     SO_USECRYPTO
#define OS_SO_USESIGNATURE  SO_USESIGNATURE
#define OS_SO_SNDBUF        SO_SNDBUF
#define OS_SO_RCVBUF        SO_RCVBUF
#define OS_SO_SNDLOWAT      SO_SNDLOWAT
#define OS_SO_RCVLOWAT      SO_RCVLOWAT
#define OS_SO_SNDTIMEO      SO_SNDTIMEO
#define OS_SO_RCVTIMEO      SO_RCVTIMEO
#define OS_SO_ERROR         SO_ERROR
#define OS_SO_TYPE          SO_TYPE
#define OS_SO_NBIO          SO_NBIO
#define OS_SO_TPPOLICY      SO_TPPOLICY
#define OS_SO_NAME          SO_NAME
#define OS_SO_ERROR_EX      SO_ERROR_EX

// Socket IO options
#define OS_MSG_PEEK         MSG_PEEK
#define OS_MSG_WAITALL      MSG_WAITALL
// Windows has no MSG_DONTWAIT equivalent...

// Socket errors
#define LASTERROR           WSAGetLastError()
#define OS_InvalidSocket    INVALID_SOCKET
#define OS_EBADF            WSAEBADF            /* File descriptor is not valid */
#define OS_EFAULT           WSAEFAULT           /* Invalid address specified */
#define OS_EINVAL           WSAEINVAL           /* Invalid argument was specified */
#define OS_ENOTSOCK        	WSAENOTSOCK        	/* Socket operation on non-socket */
#define OS_EDESTADDRREQ    	WSAEDESTADDRREQ    	/* Destination address required */
#define OS_EMSGSIZE        	WSAEMSGSIZE        	/* Message too long */
#define OS_EPROTOTYPE      	WSAEPROTOTYPE      	/* Protocol wrong type for socket */
#define OS_ENOPROTOOPT     	WSAENOPROTOOPT     	/* Protocol not available */
#define OS_EPROTONOSUPPORT 	WSAEPROTONOSUPPORT 	/* Protocol not supported */
#define OS_ESOCKTNOSUPPORT 	WSAESOCKTNOSUPPORT 	/* Socket type not supported */
#define OS_EOPNOTSUPP      	WSAEOPNOTSUPP      	/* Operation not supported on transport endpoint */
#define OS_EPFNOSUPPORT    	WSAEPFNOSUPPORT    	/* Protocol family not supported */
#define OS_EAFNOSUPPORT    	WSAEAFNOSUPPORT    	/* Address family not supported by protocol */
#define OS_EADDRINUSE      	WSAEADDRINUSE      	/* Address already in use */
#define OS_EADDRNOTAVAIL   	WSAEADDRNOTAVAIL   	/* Cannot assign requested address */
#define OS_ENETDOWN        	WSAENETDOWN        	/* Network is down */
#define OS_ENETUNREACH     	WSAENETUNREACH     	/* Network is unreachable */
#define OS_ENETRESET       	WSAENETRESET       	/* Network dropped connection because of reset */
#define OS_ECONNABORTED    	WSAECONNABORTED    	/* Software caused connection abort */
#define OS_ECONNRESET      	WSAECONNRESET      	/* Connection reset by peer */
#define OS_ENOBUFS         	WSAENOBUFS         	/* No buffer space available */
#define OS_EISCONN         	WSAEISCONN         	/* Transport endpoint is already connected */
#define OS_ENOTCONN        	WSAENOTCONN        	/* Transport endpoint is not connected */
#define OS_ESHUTDOWN       	WSAESHUTDOWN       	/* Cannot send after transport endpoint shutdown */
#define OS_ETOOMANYREFS    	WSAETOOMANYREFS    	/* Too many references: cannot splice */
#define OS_ETIMEDOUT       	WSAETIMEDOUT       	/* Connection timed out */
#define OS_ECONNREFUSED    	WSAECONNREFUSED    	/* Connection refused */
#define OS_EHOSTDOWN       	WSAEHOSTDOWN       	/* Host is down */
#define OS_EHOSTUNREACH    	WSAEHOSTUNREACH    	/* No route to host */
#define OS_EALREADY        	WSAEALREADY        	/* Operation already in progress */
#define OS_EINPROGRESS     	WSAEINPROGRESS     	/* Operation now in progress */
#define OS_EWOULDBLOCK     	WSAEWOULDBLOCK     	/* Operation would block */

// Socket termination
#define OS_SHUT_RD          SD_RECEIVE
#define OS_SHUT_WR          SD_SEND
#define OS_SHUT_RDWR        SD_BOTH



// Byte ordering
#define __bswap_64(x)                       \
  ( ((uint64_t)(htonl((uint32_t)(x))) << 32)    \
  |                                         \
     (uint64_t)(htonl((uint32_t)(x >> 32))) )
#define htonll(x)           __bswap_64(x)
#define ntohll(x)           __bswap_64(x)

//
//  File descriptor I/O & Events
//
// Workaround for FD_SET always giving warnings on windows
#define OS_FD_SET(fd, set)    \
    /* warning C4127: conditional expression is constant */ \
    __pragma( warning( suppress : 4127 )) \
    FD_SET(fd, set)

// Workaround for FD_SET always giving warnings on windows
#define OS_FD_CLR(fd, set)  \
/* warning C4127: conditional expression is constant */ \
    __pragma( warning( suppress : 4127 )) \
    FD_CLR(fd, set)