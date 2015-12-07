
#include "stdafx.h"

class cSocketMain
{
public:
	void sendPage(string url, SOCKET* client);
	void sendData(string data, SOCKET* client);
};

extern cSocketMain sock;