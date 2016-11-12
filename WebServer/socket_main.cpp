
#include "stdafx.h"
#include "socket.h"
#include "util.h"
#include <locale>
#include <codecvt>

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

wstring s2ws(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

void cSocketMain::sendData(string data, SOCKET* client)
{
	data = "HTTP/1.1 200 OK\nAccess-Control-Allow-Origin: *\n\n" + data;
	send(*client, data.c_str(), data.length(), 0);
}

void cSocketMain::sendDataToJS(wstring data, SOCKET* client)
{
	string regular = ws2s(data);
	sendData(regular, client);
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