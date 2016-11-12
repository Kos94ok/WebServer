
#include "stdafx.h"
#include "thread.h"
#include "util.h"
#include "socket.h"
#include "json.h"
#include "client.h"

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
		string url = req.substr(4, req.find(" ", 4) - 4);
		if (url == "/") { url = "/index.html"; }
		util.cout("Received GET for: " + url, 8, inet_ntoa(client_info.sin_addr), threadId);

		int found;
		string key[8], value[8];
		// Parse arguments
		if (url.find("?") != string::npos)
		{
			string args = url.substr(url.find("?") + 1);
			url = url.substr(0, url.find("?"));
			found = util.parseArguments(args, key, value);
		}

		// Normal request
		if (url.substr(0, 1) == "/" && url.find(".") != string::npos)
		{
				/*if (args.find("req=recipe_brief") != string::npos) {
					util.cout("Recipe brief request. Assembling JSON.", 6, inet_ntoa(client_info.sin_addr), threadId);

					string json, miniJson;
					json = "{";

					string cat;
					for (int i = 0; i < found; i++) {
						if (key[i] == "category") { cat = value[i]; }
					}

					int iter = 0;
					vector<string> colA;
					vector<string> colB;
					vector<string> colC;

					for (int i = 0; i < ejb.fromI("/res/db/main.db", "id"); i++) {
						string compare = ejb.from("/res/db/recipes/category.db", "id" + to_string(i));
						if (cat == compare || (cat == "all" && compare.length() > 0)) {
							string id = "id" + to_string(i);

							miniJson = "\"" + to_string(i) + "\""
								+ ":{\"name\":\"" + ejb.from("/res/db/recipes/name.db", id)
								+ "\",\"description\":\"" + ejb.from("/res/db/recipes/description.db", id)
								+ "\",\"image\":\"" + ejb.from("/res/db/recipes/image.db", id) + "\"}";
							if (iter == 0) { colA.push_back(miniJson); iter = 1; }
							else if (iter == 1) { colB.push_back(miniJson); iter = 2; }
							else if (iter == 2) { colC.push_back(miniJson); iter = 0; }
						}
					}

					json += "\"column0\":{";
					for (int i = 0; i < (int)colA.size(); i++) {
						json += colA[i];
						if (i < (int)colA.size() - 1) { json += ","; }
					}
					json += "},";
					json += "\"column1\":{";
					for (int i = 0; i < (int)colB.size(); i++) {
						json += colB[i];
						if (i < (int)colB.size() - 1) { json += ","; }
					}
					json += "},";
					json += "\"column2\":{";
					for (int i = 0; i < (int)colC.size(); i++) {
						json += colC[i];
						if (i < (int)colC.size() - 1) { json += ","; }
					}
					json += "}";

					/*for (int i = 0; i < ejb.fromI("/res/db/main.db", "id"); i++) {
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
				else if (args.find("req=recipe_text") != string::npos) {
					util.cout("Recipe whole request. Assembling JSON.", 6, inet_ntoa(client_info.sin_addr), threadId);

					string json;

					string id = "id";
					for (int i = 0; i < found; i++) {
						if (key[i] == "id") { id += value[i]; }
					}

					json = "{\"recipe\":\"" + ejb.from("/res/db/recipes/recipe.db", id) + "\"}";

					sock.sendData(json, &client);
				}*/

				sock.sendPage(url, &client, threadId, inet_ntoa(client_info.sin_addr));
		}
		// Special request
		else if (url.substr(0, 1) == "/")
		{
			// Login
			if (url == "/login")
			{
				// Return codes:
				// 0 - success
				// 1 - generic error
				// 2 - argument error
				// 3 - incorrect user credentials
				string Username = util.getArgumentValue("username", key, value, found);
				string Password = util.getArgumentValue("password", key, value, found);
				if (Username == "" || Password == "")
				{
					// No arguments found
					sock.sendData("2 [Argument error]", &client);
				}
				else
				{
					int returnCode = Client.Login(Username, Password);
					string returnData;
					if (returnCode == 0)
						returnData = "0 TestSessionKey";

					sock.sendData(returnData, &client);
				}
			}
			// Dish history
			else if (url == "/history")
			{
				string SessionKey = util.getArgumentValue("sessionKey", key, value, found);
				string returnData = Json.Assemble_DishHistory(SessionKey);
				sock.sendData(returnData, &client);
			}
		}
		// Proxy request
		else if (url.substr(0, 7) == "http://") {
			util.cout("Denying GET proxy request to \"" + url + "\"", 9, inet_ntoa(client_info.sin_addr), threadId);
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
			util.cout("Received POST for: " + url, 8, inet_ntoa(client_info.sin_addr), threadId);

			cout << req << endl;

			string args = req.substr(req.find_last_of('\n') + 1);
			string key[8];
			string value[8];
			int found = util.parseArguments(args, key, value);

			sock.sendPage(url, &client, threadId, inet_ntoa(client_info.sin_addr));
		}
		// Proxy request
		else if (url.substr(0, 7) == "http://")
		{
			util.cout("Denying POST proxy request to \"" + url + "\"", 9, inet_ntoa(client_info.sin_addr), threadId);
			sock.sendError(1000, &client);
		}
	}
	// Unknown request
	else
	{
		if (req.length() == 0 || req == " ") { req = "[NO DATA]"; }
		util.cout("Unknown request:\n" + req, 9, inet_ntoa(client_info.sin_addr), threadId);

		//sock.sendData("hi", &client);
	}
	shutdown(client, SD_SEND);
	threadData[threadId].state = THREAD_DOWN;
}