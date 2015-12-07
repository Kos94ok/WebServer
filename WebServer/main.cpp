
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

	cout << util.getTimeStr() + "Listening at port " << port << "..." << endl;
	while (true)
	{
		listen(listener, SOMAXCONN);

		SOCKET client;
		client = accept(listener, 0, 0);

		char recbuf[2048] = "";
		val = recv(client, recbuf, 2048, 0);
		if (val > 0) {
			//cout << "\nNew message:" << "\n========================================\n";
			//cout << recbuf << endl;
			//cout << "\n========================================\n";
		}

		string req = recbuf;
		// GET Request
		if (req.substr(0, 3) == "GET")
		{
			string url = req.substr(4, req.find(" ", 4) - 4), args;
			if (url == "/") { url = "/index.html"; }
			cout << util.getTimeStr() + "Received GET for: " << url << "\n";

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
						if (cat == compare) {
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

			cout << util.getTimeStr() + "Received POST for: " << url << "\n";

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
			for (int i = 0; i < found; i++) {
				cout << key[i] << " = " << util.decodeString(value[i]) << endl;
			}

			if (url == "/pages/recipes_new.html") {
				string strID = ejb.from("/res/db/main.db", "id");
				cout << strID << endl;
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
			cout << util.getTimeStr() << "Unknown request:\n";
			if (req.length() == 0 || req == " ") { req = "[NO DATA]"; }
			cout << util.getTimeStr() << req << endl;
		}
		//cout << getTimeStr() + "Terminating connection." << endl;
		shutdown(client, SD_SEND);
		//cout << "========================================" << endl;
	}

	return 0;
}