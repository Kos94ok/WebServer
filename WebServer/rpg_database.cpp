
#include "stdafx.h"
#include "rpg_database.h"
#include "rpg.h"
#include "util.h"

using namespace std;

void cRPGDatabase::Parse(string File)
{
	cRPGLocation loc("");
	cRPGDecision decision("", "");
	cRPGSelector sel;
	cRPGSelectorOption selOption;
	ifstream file;
	file.open(util.settings.mainPath + "Database/" + File, ios::binary);

	bool ParsingData = false;
	bool ParsingSelector = false;

	while (file.good())
	{
		string Line;
		getline(file, Line);
		// Data start
		if (Line.find("<LocationData>") != string::npos)
		{
			ParsingData = true;
			loc = cRPGLocation("null");
		}
		// Guid
		if (Line.find("<Guid>") != string::npos && ParsingData)
		{
			int StartIndex = Line.find("<Guid>") + 6;
			int EndIndex = Line.find("</Guid>");
			loc.guid = Line.substr(StartIndex, EndIndex - StartIndex);
		}
		// Description
		if (Line.find("<Description>") != string::npos && ParsingData)
		{
			int StartIndex = Line.find("<Description>") + 13;
			int EndIndex = Line.find("</Description>");
			loc.text = Line.substr(StartIndex, EndIndex - StartIndex);
		}
		// Decision
		if (Line.find("<LocationDecision>") != string::npos && ParsingData)
		{
			decision = cRPGDecision("", "");
			loc.decision.push_back(decision);
		}
		// Decision text
		if (Line.find("<Text>") != string::npos && ParsingData)
		{
			if (loc.decision.size() > 0)
			{
				int StartIndex = Line.find("<Text>") + 6;
				int EndIndex = Line.find("</Text>");
				loc.decision[(int)loc.decision.size() - 1].text = Line.substr(StartIndex, EndIndex - StartIndex);
			}
		}
		// Location link
		if (Line.find("<LocationLink>") != string::npos && ParsingData)
		{
			if (loc.decision.size() > 0)
			{
				int StartIndex = Line.find("<LocationLink>") + 14;
				int EndIndex = Line.find("</LocationLink>");
				loc.decision[(int)loc.decision.size() - 1].linkToLocation = Line.substr(StartIndex, EndIndex - StartIndex);
			}
		}
		// Condition
		if (Line.find("<Condition>") != string::npos && ParsingData)
		{
			if (loc.decision.size() > 0)
			{
				int StartIndex = Line.find("<Condition>") + 11;
				int EndIndex = Line.find("</Condition>");
				loc.decision[(int)loc.decision.size() - 1].condition = Line.substr(StartIndex, EndIndex - StartIndex);
			}
		}
		// Action
		if (Line.find("<Action>") != string::npos && ParsingData)
		{
			if (loc.decision.size() > 0)
			{
				int StartIndex = Line.find("<Action>") + 8;
				int EndIndex = Line.find("</Action>");
				loc.decision[(int)loc.decision.size() - 1].action = Line.substr(StartIndex, EndIndex - StartIndex);
			}
		}
		// Data end
		if (Line.find("</LocationData>") != string::npos && ParsingData)
		{
			ParsingData = false;
			location.push_back(loc);
		}

		// Selector start
		if (Line.find("<LocationSelector>") != string::npos)
		{
			ParsingSelector = true;
			sel = cRPGSelector();
		}
		// Guid
		if (Line.find("<Guid>") != string::npos && ParsingSelector)
		{
			int StartIndex = Line.find("<Guid>") + 6;
			int EndIndex = Line.find("</Guid>");
			sel.guid = Line.substr(StartIndex, EndIndex - StartIndex);
		}
		// Options
		if (Line.find("<SelectorOption>") != string::npos && ParsingSelector)
		{
			selOption = cRPGSelectorOption();
			sel.option.push_back(selOption);
		}
		// Location link
		if (Line.find("<Link>") != string::npos && ParsingSelector)
		{
			if (sel.option.size() > 0)
			{
				int StartIndex = Line.find("<Link>") + 6;
				int EndIndex = Line.find("</Link>");
				sel.option[(int)sel.option.size() - 1].link = Line.substr(StartIndex, EndIndex - StartIndex);
			}
		}
		// Chance
		if (Line.find("<Chance>") != string::npos && ParsingSelector)
		{
			if (sel.option.size() > 0)
			{
				int StartIndex = Line.find("<Chance>") + 8;
				int EndIndex = Line.find("</Chance>");
				sel.option[(int)sel.option.size() - 1].chance = atoi(Line.substr(StartIndex, EndIndex - StartIndex).c_str());
			}
		}
		// Condition
		if (Line.find("<Condition>") != string::npos && ParsingSelector)
		{
			if (sel.option.size() > 0)
			{
				int StartIndex = Line.find("<Condition>") + 11;
				int EndIndex = Line.find("</Condition>");
				sel.option[(int)sel.option.size() - 1].condition = Line.substr(StartIndex, EndIndex - StartIndex);
			}
		}
		// Action
		if (Line.find("<Action>") != string::npos && ParsingSelector)
		{
			if (sel.option.size() > 0)
			{
				int StartIndex = Line.find("<Action>") + 8;
				int EndIndex = Line.find("</Action>");
				sel.option[(int)sel.option.size() - 1].action = Line.substr(StartIndex, EndIndex - StartIndex);
			}
		}
		// Selector end
		if (Line.find("</LocationSelector>") != string::npos && ParsingSelector)
		{
			selector.push_back(sel);
		}
	}
	file.close();
}

void cRPGDatabase::Clear()
{
	this->location.clear();
}

cRPGLocation cRPGDatabase::FindLocation(string Name)
{
	for (int i = 0; i < (int)location.size(); i++)
	{
		if (location[i].guid == Name)
		{
			return location[i];
		}
	}
	return cRPGLocation("missingno");
}

cRPGSelector cRPGDatabase::FindSelector(string Name)
{
	for (int i = 0; i < (int)selector.size(); i++)
	{
		if (selector[i].guid == Name)
		{
			return selector[i];
		}
	}
	return cRPGSelector("missingno");
}

string cRPGDatabase::SelectLocation(string Selector, string SessionKey)
{
	cRPGSelector handle = FindSelector(Selector);
	if (handle.guid != "missingno")
	{
		// Check conditions and calculate total chance
		std::vector<cRPGSelectorOption> validOptions;
		int totalChances = 0, chancesLeftBehind = 0;
		for (int i = 0; i < (int)handle.option.size(); i++)
		{
			if (parent->ParseLogicalExpression(handle.option[i].condition, SessionKey))
			{
				totalChances += handle.option[i].chance;
				validOptions.push_back(handle.option[i]);
			}
		}
		// Break, if no valid options found
		if (totalChances == 0)
			return "missingno";
		// Select the location
		int roll = randomizer.getInt(0, totalChances - 1);
		// Loopy
		for (int i = 0; i < (int)validOptions.size(); i++)
		{
			chancesLeftBehind += handle.option[i].chance;
			if (roll < chancesLeftBehind)
			{
				return validOptions[i].link;
			}
		}
	}
	return "missingno";
}

void cRPGDatabase::RegisterLocation(cRPGLocation loc)
{
	location.push_back(loc);
}