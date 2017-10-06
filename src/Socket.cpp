#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "Socket.h"
#include <functional>

#define INET_IPV4_ADDRSTRLEN 15

int main(int argc, char* argv[]) 
{
	bool server = false;
	std::string addr = "127.0.0.1";
	unsigned short port = 50000;

	if (argc > 1)
	{
		for(int idxArg = 1; idxArg < argc; ++idxArg)
		{
			std::string tmp = argv[idxArg];
			int point = tmp.find_last_of(".");

			// loop on args
			size_t line = tmp.find_first_of("-");
			bool keepLoop = line != std::string::npos;
			while(keepLoop)
			{
				size_t idxEnd = tmp.find_first_of("-", line+1);

				//Another arg is available 
				if(idxEnd == std::string::npos)
				{
					keepLoop = false;
					idxEnd = tmp.size()+1;
				}
				std::string arg = tmp.substr(line, (idxEnd-1) - line);
				// Standalone mode
				if (arg == "-server")
				{
					server = true;
				}
				std::string arg_addr("-addr=");
				auto arg_addrsize = arg_addr.size();
				if (!arg.compare(0, arg_addrsize, arg_addr))
				{
					addr = arg.substr(arg_addrsize);
				}
				std::string arg_port("-port=");
				auto arg_portsize = arg_port.size();
				if (!arg.compare(0, arg_portsize, arg_port))
				{
					port = std::stoi(arg.substr(arg_portsize));
				}
				
				line = idxEnd;
			}
		}
	}

	std::function<sockaddr*(sockaddr_in&)> sockAddrCast = [](sockaddr_in& _sin)->sockaddr* {
		return static_cast<sockaddr*>(static_cast<void*>(&_sin));
	};

	OS_Init();
	OS_Sockfd socket;
	sockaddr_in sin;
	socklen_t sinSize =  sizeof(sin);

	//socket creation
	socket = OS_Socket(AF_INET, SOCK_STREAM, 0);
	if (socket == OS_InvalidSocket)
	{
		int err = WSAGetLastError();
		printf("cant initialize socket error %d\n", errno);
		exit(errno);
	}

	sin.sin_family = AF_INET;
	if (server)
	{
		OS_Sockfd clientSocket;
		sockaddr_in clientSin;
		socklen_t clientSinSize = sizeof(clientSin);
		//transform ip from host to network byte order (127.0.0.1 -> 1.0.0.127)
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
		//transform port u short from host to network byte order 
		sin.sin_port = htons(port);
		//link your socket to a communication point 
		if (OS_Bind(socket, sockAddrCast(sin), sizeof(sin)) < 0)
		{
			printf("cant bind socket\n");
			OS_CloseSocket(socket);
			exit(errno);
		}
		OS_Listen(socket, 0);
		while (true)
		{
			printf("waiting for client to connect...");
			if (clientSocket = OS_Accept(socket, sockAddrCast(clientSin), &clientSinSize))
			{
				printf("client connected, waiting for msg\n");
				char lbuf[1024];
				int size = 0;
				do {
					printf(".");
					fd_set readFDSet;
					FD_ZERO(&readFDSet);
					FD_SET(clientSocket, &readFDSet);

					int rc = OS_Select(int32_t(clientSocket + 1), &readFDSet, NULL, NULL, NULL);
					if (rc > 0 && FD_ISSET(clientSocket, &readFDSet))
					{
						size = OS_Recv(clientSocket, lbuf, 1024, 0);
						if (size > 0)
						{
							std::string msg(lbuf, size);
							printf("<< %s \n", msg.c_str());
							if (msg.compare("Hello?") == 0)
							{
								printf(">> Hi!\n");
								OS_Send(clientSocket, "Hi!", 3, 0);
							}
						}
					}
				} while (size > 0);
				OS_CloseSocket(clientSocket);
			}
		}
	}
	else
	{
		sin.sin_addr.s_addr = inet_addr(addr.c_str());
		//transform port u short from host to network byte order 
		sin.sin_port = htons(port);

		if (OS_Connect(socket, sockAddrCast(sin), sinSize) < 0)
		{
			printf("cant connect socket\n");
			OS_CloseSocket(socket);
			exit(errno);
		}

		Sleep(20000);
		OS_Send(socket, "*Wave*", 6, 0);
		Sleep(2000);
		OS_Send(socket, "Hello?", 6, 0);
		char lbuf[1024];
		int rsiz = OS_Recv(socket, lbuf, 1024, 0);
		if (rsiz <= 0)
		{
			printf("failed rsiz=%d\n", rsiz);
		}
		else
		{
			std::string msg(lbuf, rsiz);
			printf("<< %s \n", msg.c_str());
		}
	}
	OS_CloseSocket(socket);

	OS_End();
	return 0;
}