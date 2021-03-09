#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include "Socket.h"
#include <functional>
#include <thread>
#include <vector>
#include <mutex>

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
			size_t point = tmp.find_last_of(".");

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
		std::cout << "cant initialize socket error " << errno << std::endl;
		exit(errno);
	}

	sin.sin_family = AF_INET;
	if (server)
	{
		sockaddr_in clientSin;
		socklen_t clientSinSize = sizeof(clientSin);
		//transform ip from host to network byte order (127.0.0.1 -> 1.0.0.127)
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
		//transform port u short from host to network byte order 
		sin.sin_port = htons(port);
		//link your socket to a communication point 
		if (OS_Bind(socket, sockAddrCast(sin), sizeof(sin)) < 0)
		{
			std::cout << "cant bind socket" << std::endl;
			OS_CloseSocket(socket);
			exit(errno);
		}
		OS_Listen(socket, 0);
		std::vector<std::thread*> threads;
		std::mutex messMut;
		std::vector<std::string> pendingInmessages;
		std::mutex socketMut;
		std::vector<OS_Sockfd> sockets;
		bool terminate = false;
		//accept thread
		std::thread acceptThread([&]() {
			while (!terminate)
			{
				std::cout << "waiting for clients to connect..." << std::endl;
				if (OS_Sockfd clientSocket = OS_Accept(socket, sockAddrCast(clientSin), &clientSinSize))
				{
					socketMut.lock();
					sockets.push_back(clientSocket);
					socketMut.unlock();
					std::cout << "client connected, starting thread" << std::endl;
					threads.push_back(new std::thread([&terminate, clientSocket, &messMut, &pendingInmessages]() {
						char lbuf[1024];
						int size = 0;
						while (!terminate) {
							size = OS_Recv(clientSocket, lbuf, 1024, 0);
							if (size > 0)
							{
								std::string msg(lbuf, size);
								messMut.lock();
								pendingInmessages.push_back(msg);
								messMut.unlock();
							}
							else
							{
								std::cout << "client disconnected" << std::endl;
								break; //disconnected
							}
						}
						OS_CloseSocket(clientSocket);
					}));
				}
			}
		});

		while (true)
		{
			std::lock_guard<std::mutex> l(messMut);
			std::lock_guard<std::mutex> ls(socketMut);
			if (!pendingInmessages.empty())
			{
				for (const auto& str : pendingInmessages)
				{
					for (auto& sock : sockets)
					{
						//send is atomic
						OS_Send(sock, str.c_str(), str.length(),0);
					}
				}
				pendingInmessages.clear();
			}
		}
		OS_CloseSocket(socket);
	}
	else
	{
		sin.sin_addr.s_addr = inet_addr(addr.c_str());
		//transform port u short from host to network byte order 
		sin.sin_port = htons(port);

		if (OS_Connect(socket, sockAddrCast(sin), sinSize) < 0)
		{
			std::cout << "cant connect socket" << std::endl;
			OS_CloseSocket(socket);
			exit(errno);
		}
		bool exit = false;
		std::thread input([&exit, socket]() {

			std::string str;
			while (str != "EXIT")
			{
				std::cin >> str;
				OS_Send(socket, str.c_str(), str.length(), 0);
			}
			exit = true;
		});

		std::thread draw([&exit,socket]() {
			while (!exit)
			{
				char lbuf[1024];
				int rsiz = OS_Recv(socket, lbuf, 1024, 0);
				if (rsiz <= 0)
				{
					std::cout << "failed rsiz=" <<  rsiz << std::endl;
				}
				else
				{
					std::string msg(lbuf, rsiz);
					std::cout << ">>" <<  msg << std::endl;
				}
			}
		});
		input.join();
		draw.join();
		OS_CloseSocket(socket);
	}
	OS_End();
	return 0;
}