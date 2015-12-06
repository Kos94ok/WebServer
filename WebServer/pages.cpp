
#include "stdafx.h"

string decodeURL(string str)
{
	string val;
	while (str.find("%3A") != string::npos || str.find("%2F") != string::npos || str.find("%21") != string::npos)
	{
		val = str.substr(str.find("%"), 3);
		if (val == "%3A") { str.replace(str.find("%"), 3, ":"); }
		else if (val == "%2F") { str.replace(str.find("%"), 3, "/"); }
		else if (val == "%21") { str.replace(str.find("%"), 3, " "); }
	}
	return str;
}

void sendPage(string url, SOCKET* client)
{
	string strbuf, buf;
	ifstream file;
	file.open(mainpath + url, ios::in | ios::binary);
	if (file.good())
	{
		// Reading the file
		cout << getTimeStr() + "File found. Sending a response.\n";
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
					strbuf.replace(i, strbuf.find("</ejb>") - i + 6, parseEJB(scr));
				}
			}
		}
		// Send the packet
		send(*client, strbuf.c_str(), strbuf.length(), 0);
	}
	else
	{
		cout << getTimeStr() + "File not found. Sending 404.\n";
		strbuf = "HTTP/1.1 404 NOT FOUND\n\n<html><body><h1>Error 404: Page not found</h1></body></html>";
		send(*client, strbuf.c_str(), strbuf.length(), 0);
	}
	file.close();
}

void sendData(string data, SOCKET* client)
{
	send(*client, data.c_str(), data.length(), 0);
}