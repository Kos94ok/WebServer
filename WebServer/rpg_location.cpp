
#include "stdafx.h"
#include "rpg_location.h"

using namespace std;

cRPGLocation::cRPGLocation(std::string guid)
{
	this->guid = guid;
}

cRPGLocation::cRPGLocation(std::string guid, std::string text)
{
	this->guid = guid;
	this->text = text;
}

cRPGLocation::cRPGLocation(std::string guid, std::string text, std::vector<cRPGDecision> options)
{
	this->guid = guid;
	this->text = text;
	for (cRPGDecision opt : options)
		decision.push_back(opt);
}

void cRPGLocation::AddTextParagraph(string text)
{
	if (this->text.length() > 0)
		this->text.append("<br>");
	this->text.append(text);
}

void cRPGLocation::AttachDecisionList(std::vector<cRPGDecision> options)
{
	for (cRPGDecision opt : options)
		decision.push_back(opt);
}

cRPGSelector::cRPGSelector(std::string guid)
{
	this->guid = guid;
}