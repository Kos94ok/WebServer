
#pragma once

#include "stdafx.h"
#include "rpg_client.h"
#include "rpg_location.h"

class cRPG;

class cRPGNetwork
{
public:
	cRPG* parent;

	std::vector<cRPGClient> client;

	void ParseRequest(std::string req, SOCKET* client, int threadId);
	std::string ParseLocationText(std::string text, std::string sessionKey);
	bool ParseLogicalExpression(std::string exp, std::string sessionKey);
	bool CheckComparisonCondition(std::string type, std::string key, std::string value, std::string sessionKey);
	bool CheckLogicalCondition(std::string type, std::string left, std::string right, std::string sessionKey);
	bool CheckSingletonExpression(std::string exp, std::string sessionKey);
	std::string CreateLocationResponse(std::string locationName, std::string sessionKey);
	std::string CreateCharacterResponse(std::string sessionKey, std::string characterName);
	cRPGClient GetClientDataBySession(std::string sessionKey);
	cRPGClient GetClientDataByCharacterName(std::string characterName);
	std::string GenerateSessionKey();
};