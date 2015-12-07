
#include "stdafx.h"
#include "util.h"
#include "socket.h"
#include "ejb.h"

string mainpath = "../../ChristmasTime/";

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

	util.initDecoder();
	cout << util.getTimeStr() + "Listening at port " << port << "..." << endl;
	while (true)
	{
		listen(listener, SOMAXCONN);

		SOCKET client;
		sockaddr_in client_info;
		int size = sizeof(client_info);

		client = accept(listener, (sockaddr*)&client_info, &size);

		char recbuf[65536] = "";
		val = recv(client, recbuf, 65536, 0);

		string req = recbuf;
		// GET Request
		if (req.substr(0, 3) == "GET")
		{
			string url = req.substr(4, req.find(" ", 4) - 4), args;
			if (url == "/") { url = "/index.html"; }
			cout << util.getTimeStr() + "Received GET for: " << url << " [" << inet_ntoa(client_info.sin_addr) << "]\n";

			if (url.find("?") != string::npos) {
				args = url.substr(url.find("?") + 1);
				url = url.substr(0, url.find("?"));

				// Recipe data
				if (url.find("/pages/recipes.html") != string::npos) {
					cout << util.getTimeStr() << "Got it. Assembling JSON." << "\n";

					string json;
					json = "{";

					string cat = args.substr(args.find("=") + 1);
					for (int i = 0; i < ejb.fromI("/res/db/main.db", "id"); i++) {
						string compare = ejb.from("/res/db/recipes/category.db", "id" + to_string(i));
						if (cat == compare || (cat == "all" && compare.length() > 0)) {
							string id = "id" + to_string(i);
							if (json.length() > 1) { json += ","; }

							json += "\"" + to_string(i) + "\""
								+ ":{\"name\":\"" + ejb.from("/res/db/recipes/name.db", id)
								+ "\",\"description\":\"" + ejb.from("/res/db/recipes/description.db", id)
								+ "\",\"recipe\":\"" + ejb.from("/res/db/recipes/recipe.db", id)
								+ "\",\"image\":\"" + ejb.from("/res/db/recipes/image.db", id) + "\"}";
						}
					}
					json += "}";
					sock.sendData(json, &client);
				}
			}
			else {
				sock.sendPage(url, &client);
			}
		}
		// POST Request
		else if (req.substr(0, 4) == "POST")
		{
			string url = req.substr(5, req.find(" ", 5) - 5);
			if (url == "/") { url = "/index.html"; }

			cout << util.getTimeStr() + "Received POST for: " << url << " [" << inet_ntoa(client_info.sin_addr) << "]\n";

			string args = req.substr(req.find_last_of('\n') + 1);
			string key[8];
			string value[8];
			int found = 0;
			while (true)
			{
				key[found] = args.substr(0, args.find("="));
				if (args.find("&") != string::npos) {
					if (args.find("&") != args.find("=") + 1) {
						value[found] = args.substr(args.find("=") + 1, args.find("&") - args.find("=") - 1);
					}
					else {
						value[found] = "";
					}
					args.erase(0, args.find("&") + 1);
					found += 1;
				}
				else {
					value[found] = args.substr(args.find("=") + 1);
					found += 1;
					break;
				}
			}
			if (url == "/pages/recipes_new.html") {
				string strID = ejb.from("/res/db/main.db", "id");
				int id;
				stringstream(strID) >> id;
				cout << "Pushing new data with id [" << id << "]" << endl;

				for (int i = 0; i < found; i++) {
					ejb.push("/res/db/recipes/" + key[i] + ".db", "id" + to_string(id), util.decodeString(value[i]));
				}

				ejb.push("/res/db/main.db", "id", to_string(id + 1));
			}

			sock.sendPage(url, &client);
		}
		else {
			cout << util.getTimeStr() << "Unknown request" << " [" << inet_ntoa(client_info.sin_addr) << "]:\n";;
			if (req.length() == 0 || req == " ") { req = "[NO DATA]"; }
			cout << util.getTimeStr() << req << "\n";

			sock.sendData("hi", &client);
		}
		shutdown(client, SD_SEND);
	}

	return 0;
}