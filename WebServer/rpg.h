
#include "stdafx.h"
#include <vector>
#include "rpg_client.h"
#include "rpg_location.h"
#include "rpg_database.h"
#include "rpg_network.h"
#include "rpg_world.h"

class cRPG
{
private:
	cRPGDatabase db;
	cRPGNetwork network;
	cRPGWorld world;

public:
	void Initialization();
	cRPGLocation FindLocation(std::string Name)									{ return db.FindLocation(Name); }
	cRPGSelector FindSelector(std::string Name)									{ return db.FindSelector(Name); }
	std::string SelectLocation(std::string Selector, std::string SessionKey)	{ return db.SelectLocation(Selector, SessionKey); }
	bool ParseLogicalExpression(std::string exp, std::string sessionKey)		{ return network.ParseLogicalExpression(exp, sessionKey); }
	void ParseDatabase()														{ db.Clear(); db.Parse("locations.xml"); }
	void ParseRequest(std::string req, SOCKET* client, int threadId)			{ network.ParseRequest(req, client, threadId); }
	cRPGWorldData GetWorldData()												{ return world.Data; }
};

extern cRPG rpg;