
#include "stdafx.h"
#include "socket.h"
#include "util.h"
#include "ejb.h"

using namespace std;

cSocketMain sock;

void cSocketMain::sendPage(string url, SOCKET* client, int threadId, string ip)
{
	string strbuf, buf;
	ifstream file;
	file.open(util.settings.mainPath + url, ios::in | ios::binary);
	if (file.good())
	{
		// Reading the file
		util.cout("File \"" + url + "\" found. Sending content.", 7, ip, threadId);
		strbuf.clear();
		ostringstream oss;
		oss << "HTTP/1.1 200 OK\n\n";
		oss << file.rdbuf();
		strbuf = oss.str();
		// Parsing EJB script
		if (url.length() > 5 && url.substr(url.length() - 5, 5) == ".html")
		{
			for (int i = 0; i < (int)strbuf.length() - 5; i++) {
				if (strbuf.substr(i, 5) == "<ejb>") {
					string scr = strbuf.substr(i + 5, strbuf.find("</ejb>") - i - 5);
					strbuf.replace(i, strbuf.find("</ejb>") - i + 6, ejb.parse(scr));
				}
			}
		}
		// Send the packet
		send(*client, strbuf.c_str(), strbuf.length(), 0);
	}
	else
	{
		util.cout("File \"" + url + "\" not found. Sending 404.", 10, ip, threadId);
		sendError(404, client);
	}
	file.close();
}

void cSocketMain::sendData(string data, SOCKET* client)
{
	send(*client, data.c_str(), data.length(), 0);
}

void cSocketMain::sendError(int code, SOCKET* client)
{
	// HTTP header
	string buf = "HTTP/1.1 ";

	// Error code
	if (code == 404) { buf += "404 NOT FOUND\n\n"; }
	else if (code == 1000) { buf += "1000 NOT PROXY\n\n"; }

	// HTML Body
	buf += "<html><body><h1>";

	if (code == 404) { buf += "Error 404: Page not found"; }
	else if (code == 1000) { buf += "ME NO PROXY"; }

	buf += "</h1></body></html>";

	send(*client, buf.c_str(), buf.length(), 0);
}