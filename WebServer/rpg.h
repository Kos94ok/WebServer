
#include "stdafx.h"
#include <vector>
#include <list>
#include "rpg_client.h"
#include "rpg_location.h"

class cRPG
{
public:
	std::vector<cRPGClient> client;
	std::vector<cRPGLocation> location;

	void Initialization();
	void RegisterLocation(cRPGLocation loc);
	void ParseRequest(std::string req, SOCKET* client);
};

extern cRPG rpg;