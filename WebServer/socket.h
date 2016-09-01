
#include "stdafx.h"

class cSocketMain
{
public:
	void sendPage(std::string url, SOCKET* client, int threadId, std::string ip);
	void sendData(std::string data, SOCKET* client);
	void sendDataToJS(std::wstring data, SOCKET* client);
	void sendError(int code, SOCKET* client);
};

extern cSocketMain sock;