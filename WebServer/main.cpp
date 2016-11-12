
#include "stdafx.h"
#include "util.h"
#include "socket.h"
#include "thread.h"
#include "client.h"
#include "json.h"

void parseConsoleCommand(std::string cmd)
{
	if (cmd == "hide") { ShowWindow(GetConsoleWindow(), SW_HIDE); }
	else if (cmd == "show") { ShowWindow(GetConsoleWindow(), SW_SHOW); }
}

void fileConsole()
{
	std::ifstream file("console");
	if (!file.good()) { return; }
	while (true)
	{
		std::string buf;
		file.open("console");
		if (!file.eof()) {
			std::getline(file, buf);
			parseConsoleCommand(buf);
			file.close();
			std::ofstream clr("console");
			clr.close();
		}
		else {
			file.close();
		}
		Sleep(1000);
	}
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "");
	std::vector<std::thread> threadPool;

	util.settings.load();
	std::thread fileConsole(fileConsole);

	// Arguments
	for (int i = 1; i < argc; i++) {
		parseConsoleCommand(argv[i]);
	}

	// Clear log
	if (util.settings.enableLog == 1) {
		std::ofstream file("log.txt");
		file << "Hi, I'm a log." << "\n";
		file.close();
	}

	// Check index
	std::ifstream indexTest(util.settings.mainPath + "index.html");
	if (indexTest.good()) {
		util.cout("Index found at \"" + util.settings.mainPath + "index.html" + "\"", 16);
		indexTest.close();
	}
	else {
		util.cout("Can't find index at \"" + util.settings.mainPath + "index.html" + "\"", 16);
		getchar();
		return 0;
	}

	Client.SeedData();

	// Test
	util.cout("Json: " + Json.Assemble_DishHistory("TestSessionKey"));
	// Stop test

	// Initialize sockets
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	int val = 0;
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	std::string port = std::to_string(util.settings.mainPort);
	// Resolve the local address and port to be used by the server
	getaddrinfo(NULL, port.c_str(), &hints, &result);

	SOCKET listener = INVALID_SOCKET;
	listener = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (val == INVALID_SOCKET) { std::cout << "Invalid socket" << std::endl; getchar(); return 0; }
	val = bind(listener, result->ai_addr, result->ai_addrlen);
	if (val == SOCKET_ERROR) { printf("Error at socket(): %ld\n", WSAGetLastError()); getchar(); return 0; }
	freeaddrinfo(result);

	using namespace std;

	util.initDecoder();
	util.cout("Listening at port " + port + "...", 16);
	while (true)
	{
		listen(listener, SOMAXCONN);

		SOCKET client;
		sockaddr_in client_info;
		int size = sizeof(client_info);

		client = accept(listener, (sockaddr*)&client_info, &size);

		float curtime = (float)timeGetTime();
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