
#include "stdafx.h"

#define THREAD_DOWN				0
#define THREAD_UP				1

class cThreadData
{
public:
	int state;
	int timestamp;
};

extern std::vector<cThreadData> threadData;
void clientThread(SOCKET client, sockaddr_in client_info, int threadId);