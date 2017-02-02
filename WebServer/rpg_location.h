
#pragma once

#include "stdafx.h"
#include <vector>
#include <string>
#include "rpg_object.h"

class cRPGDecision : public cRPGObject
{
public:
	std::string text = "";
	std::string linkToLocation = "";
	std::string condition = "";
	std::string action = "";

	cRPGDecision(std::string text, std::string link)
	{
		this->text = text;
		this->linkToLocation = link;
	}
};

class cRPGLocation : public cRPGObject
{
public:
	std::string text = "";
	std::vector<cRPGDecision> decision;

	cRPGLocation(std::string guid);
	cRPGLocation(std::string guid, std::string text);
	cRPGLocation(std::string guid, std::string text, std::vector<cRPGDecision> options);
	void AddTextParagraph(std::string text);
	void AttachDecisionList(std::vector<cRPGDecision> options);
};

class cRPGSelectorOption : public cRPGObject
{
public:
	std::string link;
	int chance;
	std::string condition;
	std::string action;
};

class cRPGSelector : public cRPGObject
{
public:
	std::vector<cRPGSelectorOption> option;

	cRPGSelector() {};
	cRPGSelector(std::string guid);
};