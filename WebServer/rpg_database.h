
#pragma once

#include "stdafx.h"
#include "rpg_location.h"

class cRPG;

class cRPGDatabase
{
public:
	cRPG* parent;

	std::vector<cRPGLocation> location;
	std::vector<cRPGSelector> selector;

	void Parse(std::string File);
	void Clear();
	cRPGLocation FindLocation(std::string Name);
	cRPGSelector FindSelector(std::string Name);
	std::string SelectLocation(std::string Selector, std::string SessionKey);
	void RegisterLocation(cRPGLocation loc);
};