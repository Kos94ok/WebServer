
#include "stdafx.h"
#include "rpg_location.h"

using namespace std;

cRPGLocation::cRPGLocation(std::string guid)
{
	this->guid = guid;
}

cRPGLocation::cRPGLocation(std::string guid, std::wstring text)
{
	this->guid = guid;
	this->text = text;
}

cRPGLocation::cRPGLocation(std::string guid, std::wstring text, std::vector<cRPGDecision> options)
{
	this->guid = guid;
	this->text = text;
	for (cRPGDecision opt : options)
		decision.push_back(opt);
}

void cRPGLocation::AddTextParagraph(wstring text)
{
	if (this->text.length() > 0)
		this->text.append(L"<br>");
	this->text.append(text);
}

void cRPGLocation::AttachDecisionList(std::vector<cRPGDecision> options)
{
	for (cRPGDecision opt : options)
		decision.push_back(opt);
}