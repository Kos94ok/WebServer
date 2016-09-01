
#include "stdafx.h"
#include <vector>
#include <string>
#include "rpg_core.h"

class cRPGDecision : public cRPGObject
{
public:
	std::wstring text = L"";
	std::string linkToLocation = "";

	cRPGDecision(std::wstring text, std::string link)
	{
		this->text = text;
		this->linkToLocation = link;
	}
};

class cRPGLocation : public cRPGObject
{
public:
	std::wstring text = L"";
	std::vector<cRPGDecision> decision;

	cRPGLocation(std::string guid);
	cRPGLocation(std::string guid, std::wstring text);
	cRPGLocation(std::string guid, std::wstring text, std::vector<cRPGDecision> options);
	void AddTextParagraph(std::wstring text);
	void AttachDecisionList(std::vector<cRPGDecision> options);
};