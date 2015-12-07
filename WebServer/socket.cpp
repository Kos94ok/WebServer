
#include "stdafx.h"
#include "socket.h"
#include "util.h"
#include "ejb.h"

cSocketMain sock;

void cSocketMain::sendPage(string url, SOCKET* client)
{
	string strbuf, buf;
	ifstream file;
	file.open(mainpath + url, ios::in | ios::binary);
	if (file.good())
	{
		// Reading the file
		cout << util.getTimeStr() + "File found. Sending a response.\n";
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
		cout << util.getTimeStr() + "File not found. Sending 404.\n";
		strbuf = "HTTP/1.1 404 NOT FOUND\n\n<html><body><h1>Error 404: Page not found</h1></body></html>";
		send(*client, strbuf.c_str(), strbuf.length(), 0);
	}
	file.close();
}

void cSocketMain::sendData(string data, SOCKET* client)
{
	send(*client, data.c_str(), data.length(), 0);
}