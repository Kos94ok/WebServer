
#include "stdafx.h"
#include "util.h"
#include "socket.h"
#include "ejb.h"
#include "thread.h"

std::string mainpath = "../../ChristmasTime/";

int main()
{
	std::vector<std::thread> threadPool;

	util.settings.load();
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	int val = 0;
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	std::string port = "8080";
	// Resolve the local address and port to be used by the server
	getaddrinfo(NULL, port.c_str(), &hints, &result);

	SOCKET listener = INVALID_SOCKET;
	listener = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (val == INVALID_SOCKET) { std::cout << "Invalid socket" << std::endl; getchar(); }
	val = bind(listener, result->ai_addr, result->ai_addrlen);
	if (val == SOCKET_ERROR) { printf("Error at socket(): %ld\n", WSAGetLastError()); getchar(); }
	freeaddrinfo(result);

	using namespace std;

	util.initDecoder();
	cout << util.getTimeStr() + "Listening at port " << port << "..." << endl;
	while (true)
	{
		listen(listener, SOMAXCONN);

		SOCKET client;
		sockaddr_in client_info;
		int size = sizeof(client_info);

		client = accept(listener, (sockaddr*)&client_info, &size);

		float curtime = (int)timeGetTime();
		if (threadData.size() > 0)
		{
			while (threadData[(int)threadData.size() - 1].state == THREAD_DOWN)
			{
				int i = (int)threadData.size() - 1;
				threadPool[i].join();
				//threadData.erase(threadData.begin() + i, threadData.begin() + i + 1);
				//threadPool.erase(threadPool.begin() + i, threadPool.begin() + i + 1);
				threadData.pop_back();
				threadPool.pop_back();
				//util.cout("Thread " + to_string(i) + " operating for " + to_string((curtime - threadData[i].timestamp) / 1000.f) + " seconds.");
			}
		}

		cThreadData dt;
		dt.state = THREAD_UP;
		dt.timestamp = timeGetTime();
		threadData.push_back(dt);
		threadPool.push_back(thread(clientThread, client, client_info, (int)threadPool.size()));
	}

	return 0;
}