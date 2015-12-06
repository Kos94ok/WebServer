#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

string mainpath = "F:/Workspace/Website/ChristmasTime";

#pragma comment(lib, "Ws2_32.lib")

string getTimeStr()
{
	string val = "";
	time_t t = time(0);
	struct tm now;
	localtime_s(&now, &t);

	if (now.tm_hour < 10) { val += "0"; }
	val += to_string(now.tm_hour) + ":";
	if (now.tm_min < 10) { val += "0"; }
	val += to_string(now.tm_min) + ":";
	if (now.tm_sec < 10) { val += "0"; }
	val += to_string(now.tm_sec);
	val += " ";
	return val;
}

string parseEJB(string script)
{
	string retval = "", parse;
	string cmd = script.substr(0, script.find(":"));
	string args = script.substr(script.find(":") + 2);
	if (cmd == "from")
	{
		string filename = args.substr(0, args.find(" "));
		string entry = args.substr(args.find(" ") + 1);
		ifstream file(mainpath + filename);
		if (file.good())
		{
			while (!file.eof())
			{
				char buf[256];
				file.getline(buf, 256);
				parse = buf;
				if (parse.substr(0, parse.find(" = ")) == entry) {
					retval = parse.substr(parse.find(" = ") + 3);
					break;
				}
			}
		}
		file.close();
	}
	else if (cmd == "flush")
	{
		string filename = args.substr(0, args.find(" "));
		ifstream file(mainpath + filename);
		if (file.good())
		{
			ostringstream oss;
			oss << file.rdbuf();
			retval = oss.str();
		}
		file.close();
	}
	return retval;
}

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	int val = 0;
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	string port = "8080";
	// Resolve the local address and port to be used by the server
	getaddrinfo(NULL, port.c_str(), &hints, &result);

	SOCKET listener = INVALID_SOCKET;
	listener = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (val == INVALID_SOCKET) { cout << "Invalid socket" << endl; getchar(); }
	val = bind(listener, result->ai_addr, result->ai_addrlen);
	if (val == SOCKET_ERROR) { printf("Error at socket(): %ld\n", WSAGetLastError()); getchar(); }
	freeaddrinfo(result);

	cout << getTimeStr() + "Listening at port " << port << "..." << endl;
	while (true)
	{
		listen(listener, SOMAXCONN);

		SOCKET client;
		client = accept(listener, 0, 0);

		char recbuf[2048] = "";
		val = recv(client, recbuf, 2048, 0);
		if (val > 0) {
			cout << "\nNew message:" << "\n========================================\n";
			//cout << recbuf << endl;
			//cout << "\n========================================\n";
		}

		string req = recbuf;
		// GET Request
		if (req.substr(0, 3) == "GET")
		{
			string url = req.substr(4, req.find(" ", 4) - 4);
			if (url == "/") { url = "/index.html"; }
			cout << getTimeStr() + "Received GET for: " << url << endl;

			string strbuf, buf;
			ifstream file;
			file.open(mainpath + url, ios::in | ios::binary);
			if (file.good())
			{
				// Reading the file
				cout << getTimeStr() + "File found. Creating a response." << endl;
				strbuf.clear();
				ostringstream oss;
				oss << file.rdbuf();
				strbuf = oss.str();
				// Parsing EJB script
				if (url.length() > 5 && url.substr(url.length() - 5, 5) == ".html")
				{
					for (int i = 0; i < (int)strbuf.length() - 5; i++) {
						if (strbuf.substr(i, 5) == "<ejb>") {
							cout << getTimeStr() + "EJB script detected. Parsing." << endl;
							string scr = strbuf.substr(i + 5, strbuf.find("</ejb>") - i - 5);

							strbuf.replace(i, strbuf.find("</ejb>") - i + 6, parseEJB(scr));
							cout << getTimeStr() + "EJB script parsed successfuly." << endl;
						}
					}
				}
				// Send the packet
				cout << getTimeStr() + "Sending response packet." << endl;
				send(client, strbuf.c_str(), strbuf.length(), 0);
			}
			else
			{
				cout << getTimeStr() + "File not found. Sending 404." << endl;
				strbuf = "HTTP/1.1 404 NOT FOUND\n\n<html><body><h1>Error 404: Page not found</h1></body></html>";
				send(client, strbuf.c_str(), strbuf.length(), 0);
			}
			file.close();
		}
		cout << getTimeStr() + "Terminating connection." << endl;
		shutdown(client, SD_SEND);
		cout << "========================================" << endl;
	}

	return 0;
}