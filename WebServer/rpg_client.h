
#pragma once

#include "stdafx.h"

class cRPGClient
{
public:
	std::string playerName = "";
	std::string sessionKey = "";
	std::string currentLocation = "";

	std::string characterGender = "";
	std::string characterRace = "";
	
	cRPGClient() {}
	cRPGClient(std::string sessionKey);
	void MoveToLocation(std::string location);
};