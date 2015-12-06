
#include "stdafx.h"

string mainpath = "../../ChristmasTime/";

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

int main()
{
	cout << ejb_from("/res/db/recipes/category.ini", "id0") << endl;

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
			cout << getTimeStr() + "Received GET for: " << url << "\n";

			if (url.find("?") != string::npos) {
				args = url.substr(url.find("?") + 1);
				url = url.substr(0, url.find("?"));

				string json;
				json = "{";

				string cat = args.substr(args.find("=") + 1);
				for (int i = 0; i < ejb_fromI("/res/db/main.db", "id"); i++) {
					cout << "id" + to_string(i) << endl;
					string compare = ejb_from("/res/db/recipes/category.db", "id" + to_string(i));
					//cout << "COMP: " << cat << " / " << compare << endl;
					cout << ejb_from("/res/db/recipes/category.db", "id0") << endl;
					if (compare == cat) {
						string id = "id" + to_string(i);
						if (json.length() > 1) { json += ","; }
						json += to_string(i)
							+ ":{name:" + ejb_from("/res/db/recipes/name.db", id)
							+ ",description:" + ejb_from("/res/db/recipes/description.db", id)
							+ ",recipe:" + ejb_from("/res/db/recipes/recipe.db", id)
							+ ",image:" + ejb_from("/res/db/recipes/image.db", id) + "}";
					}
				}
				json += "}";
				cout << json << endl;
				sendData(json, &client);
			}
			else {
				sendPage(url, &client);
			}
		}
		// POST Request
		else if (req.substr(0, 4) == "POST")
		{
			string url = req.substr(5, req.find(" ", 5) - 5);
			if (url == "/") { url = "/index.html"; }

			cout << getTimeStr() + "Received POST for: " << url << "\n";

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
				cout << key[i] << " = " << decodeURL(value[i]) << endl;
			}

			if (url == "/pages/recipes.html") {
				string strID = ejb_from("/res/db/main.db", "id");
				int id;
				stringstream(strID) >> id;
				cout << "Pushing new data with id [" << id << "]" << endl;

				for (int i = 0; i < found; i++) {
					ejb_push("/res/db/recipes/" + key[i] + ".db", "id" + to_string(id), value[i]);
				}

				ejb_push("/res/db/main.db", "id", to_string(id + 1));
			}

			sendPage(url, &client);
		}
		else {
			cout << getTimeStr() << "Unknown request:\n";
			cout << getTimeStr() << req << endl;
		}
		//cout << getTimeStr() + "Terminating connection." << endl;
		shutdown(client, SD_SEND);
		//cout << "========================================" << endl;
	}

	return 0;
}