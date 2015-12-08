
#include "stdafx.h"
#include "thread.h"
#include "util.h"
#include "socket.h"
#include "ejb.h"

std::vector<cThreadData> threadData;

void clientThread(SOCKET client, sockaddr_in client_info, int threadId)
{
	using namespace std;

	char recbuf[65536] = "";
	recv(client, recbuf, 65536, 0);

	string req = recbuf;
	// GET Request
	if (req.substr(0, 3) == "GET")
	{
		string url = req.substr(4, req.find(" ", 4) - 4), args;
		if (url == "/") { url = "/index.html"; }
		util.cout("Received GET for: " + url + " [" + inet_ntoa(client_info.sin_addr) + "]", 8, threadId);

		// Normal request
		if (url.substr(0, 1) == "/")
		{
			if (url.find("?") != string::npos) {
				args = url.substr(url.find("?") + 1);
				url = url.substr(0, url.find("?"));

				// Recipe data
				if (url.find("/pages/recipes.html") != string::npos) {
					util.cout("Got it. Assembling JSON.", 6, threadId);

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
				sock.sendPage(url, &client, threadId);
			}
		}
		// Proxy request
		else if (url.substr(0, 7) == "http://") {
			util.cout("Denying GET proxy request to \"" + url + "\" [" + inet_ntoa(client_info.sin_addr) + "]", 9, threadId);
			sock.sendError(1000, &client);
		}
	}
	// POST Request
	else if (req.substr(0, 4) == "POST")
	{
		string url = req.substr(5, req.find(" ", 5) - 5);
		if (url == "/") { url = "/index.html"; }

		// Normal request
		if (url.substr(0, 1) == "/")
		{
			util.cout("Received POST for: " + url + " [" + inet_ntoa(client_info.sin_addr) + "]", 8, threadId);

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
				util.cout("Pushing new data with id [" + to_string(id) + "]", 7, threadId);

				for (int i = 0; i < found; i++) {
					ejb.push("/res/db/recipes/" + key[i] + ".db", "id" + to_string(id), util.decodeString(value[i]));
				}

				ejb.push("/res/db/main.db", "id", to_string(id + 1));
			}

			sock.sendPage(url, &client, threadId);
		}
		// Proxy request
		else if (url.substr(0, 7) == "http://") {
			util.cout("Denying POST proxy request to \"" + url + "\" [" + inet_ntoa(client_info.sin_addr) + "]", 9, threadId);
			sock.sendError(1000, &client);
		}
	}
	// Unknown request
	else {
		if (req.length() == 0 || req == " ") { req = "[NO DATA]"; }
		util.cout("Unknown request [" + string(inet_ntoa(client_info.sin_addr)) + "]:\n" + req, 9, threadId);

		sock.sendData("hi", &client);
	}
	shutdown(client, SD_SEND);
	threadData[threadId].state = THREAD_DOWN;
}