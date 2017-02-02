
#include "stdafx.h"
#include "rpg_network.h"
#include "util.h"
#include "rpg.h"
#include "socket.h"
#include <stack>

using namespace std;

void cRPGNetwork::ParseRequest(std::string req, SOCKET* client, int threadId)
{
	string args = req.substr(req.find_last_of('\n') + 1);
	string key[8];
	string value[8];
	int found = util.parseArguments(args, key, value);

	string response = "";
	string fetchedLocation = "";
	//====================================================
	// New player session
	//====================================================
	if (key[0] == "req" && value[0] == "entry")
	{
		string requestedModule = value[1];
		string playerName = value[2];
		string characterRace = value[3];
		string characterGender = value[4];
		// Select a starting location
		if (requestedModule == "TestLoc")
			fetchedLocation = "TestLoc_Entry";
		else if (requestedModule == "Development")
			fetchedLocation = "Development_Entry";
		else if (requestedModule == "Void")
			fetchedLocation = "Void";

		// Removing old sessions
		for (int i = 0; i < (int)this->client.size(); i++)
		{
			if (this->client[i].playerName == playerName)
			{
				this->client.erase(this->client.begin() + i);
				i -= 1;
			}
		}

		// Generate random session key
		string sessionKey = GenerateSessionKey();
		cRPGClient player(sessionKey);
		player.MoveToLocation(fetchedLocation);
		player.playerName = playerName;
		player.characterRace = characterRace;
		player.characterGender = characterGender;
		this->client.push_back(player);

		response += "session=" + sessionKey + "%%";

		if (playerName.length() > 0)
			util.cout("Created session for player " + playerName, 7, "Key " + sessionKey.substr(0, 6), threadId);
		else
			util.cout("Created session for unnamed player", 7, "Key " + sessionKey.substr(0, 6), threadId);
	}
	//====================================================
	// Resume session
	//====================================================
	if (key[0] == "req" && value[0] == "resume")
	{
		string playerName = value[1];
		for (int i = 0; i < (int)this->client.size(); i++)
		{
			if (this->client[i].playerName == playerName)
			{
				fetchedLocation = this->client[i].currentLocation;

				string sessionKey = GenerateSessionKey();
				this->client[i].sessionKey = sessionKey;
				response += "session=" + sessionKey + "%%";

				util.cout("Resumed session for player " + playerName, 7, "Key " + sessionKey.substr(0, 6), threadId);
			}
		}
	}
	//====================================================
	// An action is being performed
	//====================================================
	else if (key[0] == "req" && value[0] == "decision")
	{
		int playerIndex;
		string playersLocation;
		string sessionKey = value[1];
		string clientsLocation = value[2];
		string selectedDecision = value[3];
		bool sessionValid = false;
		for (int i = 0; i < (int)this->client.size(); i++)
		{
			if (this->client[i].sessionKey == sessionKey)
			{
				sessionValid = true;
				playerIndex = i;
				playersLocation = this->client[i].currentLocation;
				break;
			}
		}
		// Session expired
		if (!sessionValid)
		{
			fetchedLocation = "Error_SessionExpired";
			response += CreateLocationResponse(fetchedLocation, sessionKey);
		}
		// Check if client's expected location matches with the real one
		if (clientsLocation == playersLocation)
		{
			util.cout("Decision " + selectedDecision + " at " + playersLocation, 7, "Key " + sessionKey.substr(0, 6), threadId);
			cRPGLocation locationHandle = parent->FindLocation(playersLocation);
			if (locationHandle.guid != "missingno")
			{
				if ((int)locationHandle.decision.size() > atoi(selectedDecision.c_str()))
				{
					fetchedLocation = locationHandle.decision[atoi(selectedDecision.c_str())].linkToLocation;
					// If link leads to selector
					if (parent->FindSelector(fetchedLocation).guid != "missingno")
					{
						fetchedLocation = parent->SelectLocation(fetchedLocation, sessionKey);
					}
					// Then, find the location. The conditions and actions are already taken care of.
					if (parent->FindLocation(fetchedLocation).guid == "missingno")
						fetchedLocation = playersLocation;

					this->client[playerIndex].MoveToLocation(fetchedLocation);
					response += "isCharacterUpdated=false%%";
					response += CreateLocationResponse(fetchedLocation, sessionKey);
				}
				else
					util.cout("Illegal move from " + locationHandle.guid, 8, "Key " + sessionKey.substr(0, 6), threadId);
			}
			util.cout("Moved to " + fetchedLocation, 6, "Key " + sessionKey.substr(0, 6), threadId);
		}
		else
		{
			response += CreateLocationResponse(playersLocation, sessionKey);
			util.cout("Player got confused. Sending info for " + fetchedLocation, 6, "Key " + sessionKey.substr(0, 6), threadId);
		}
	}
	//====================================================
	// Generic location data request
	//====================================================
	else if (key[0] == "req" && value[0] == "locationData")
	{
		string sessionKey = value[1];
		cRPGClient player = GetClientDataBySession(sessionKey);
		if (player.playerName != "missingno")
			response += CreateLocationResponse(player.currentLocation, sessionKey);
	}
	//====================================================
	// Character info request
	//====================================================
	else if (key[0] == "req" && value[0] == "characterData")
	{
		string sessionKey = value[1];
		string targetCharacter = value[2];
		cRPGClient player = GetClientDataBySession(sessionKey);
		if (player.playerName != "missingno")
			response += CreateCharacterResponse(sessionKey, targetCharacter);
	}
	//====================================================
	// World info request
	//====================================================
	else if (key[0] == "req" && value[0] == "worldData")
	{
		response += "time=" + parent->GetWorldData().Time;
	}

	//====================================================
	// Replying
	//====================================================
	if (response.length() > 0)
	{
		response = "Content-Type: text/plain; charset=utf-8\n\n" + response;
		response = "Connection: close\n" + response;
		response = "HTTP/1.1 200 OK\n" + response;
		sock.sendData(response, client);
	}
}

class cDescriptionTag
{
public:
	std::string type = "";
	std::string argument = "";
	std::string content = "";
	int startingIndex = -1;

	cDescriptionTag(){};
};

string cRPGNetwork::ParseLocationText(string text, string sessionKey)
{
	cRPGClient Client = GetClientDataBySession(sessionKey);
	cRPGWorldData World = parent->GetWorldData();
	if (Client.sessionKey == "missingno")
		return text;

	// Hopefully good approach
	stack<cDescriptionTag> TagStack;
	int Lifeguard = 1000;
	int TagPos = -1;
	while (text.find("{", TagPos + 1) != string::npos && Lifeguard > 0)
	{
		Lifeguard -= 1;
		cDescriptionTag Tag = cDescriptionTag();
		TagPos = text.find("{", TagPos + 1);
		// Enter if-statement
		if (text.substr(TagPos, 3) == "{if")
		{
			Tag.type = "if";
			Tag.argument = text.substr(TagPos + 4, text.find("}", TagPos) - TagPos - 4);
			Tag.startingIndex = TagPos;
			TagStack.push(Tag);
		}
		// Else if-statement
		else if (text.substr(TagPos, 7) == "{elseif")
		{
			Tag.type = "elseif";
			Tag.argument = text.substr(TagPos + 8, text.find("}", TagPos) - TagPos - 8);
			Tag.startingIndex = TagPos;
			TagStack.push(Tag);
		}
		// Else-statement
		else if (text.substr(TagPos, 6) == "{else}")
		{
			Tag.type = "else";
			Tag.startingIndex = TagPos;
			TagStack.push(Tag);
		}
		// End if-statement
		else if (text.substr(TagPos, 7) == "{endif}")
		{
			stack<cDescriptionTag> Statement;
			// Fetching all the tags
			while (true)
			{
				cDescriptionTag StackEntry = TagStack.top();	TagStack.pop();
				Statement.push(StackEntry);
				if (StackEntry.type == "if")
					break;
			}
			// Some variables
			string TagOutput;
			bool SkipRemaining = false;
			// Parsing the tags
			int IfStart = Statement.top().startingIndex;
			int IfEnd = TagPos + 7;

			while (Statement.size() > 0)
			{
				cDescriptionTag StackEntry = Statement.top();	Statement.pop();
				if (ParseLogicalExpression(StackEntry.argument, sessionKey) == true)
				{
					int ContentStart = text.find("}", StackEntry.startingIndex) + 1;
					int ContentEnd;
					if (Statement.size() > 0)
						ContentEnd = Statement.top().startingIndex;
					else
						ContentEnd = TagPos;
					TagOutput = text.substr(ContentStart, ContentEnd - ContentStart);
					break;
				}
			}
			text.erase(IfStart, IfEnd - IfStart);
			text.insert(IfStart, TagOutput);
			TagPos = IfStart - 1;
		}
	}

	// Fix line breaks
	while (text.find("<br><br>") != string::npos)
	{
		text = text.erase(text.find("<br><br>"), 4);
	}

	int startIndex = 0;
	while (text.find("<p>", startIndex) != string::npos)
	{
		int paraPos = text.find("<p>", startIndex);
		if (text.substr(paraPos + 3, 4) == "<br>")
			text.erase(paraPos + 3, 4);
		int paraEnd = text.find("</p>", startIndex);
		if (paraEnd != string::npos && paraEnd >= 4)
		{
			if (text.substr(paraEnd - 4, 4) == "<br>")
				text.erase(paraEnd - 4, 4);
		}

		startIndex = paraEnd + 4;
	}

	// Return
	return text;
}

bool cRPGNetwork::ParseLogicalExpression(string exp, string sessionKey)
{
	if (exp == "")
		return true;
	
	/*while (exp != "true" && exp != "false")
	{
		string nextSeparator;
		if (exp.find("||") == string::npos || exp.find("&&") < exp.find("||"))
			nextSeparator = "&&";
		else if (exp.find("||") != string::npos)
			nextSeparator = "||";

		string left = exp.substr(0, exp.find(nextSeparator));
		string right = exp.substr(exp.find(nextSeparator) + 2);

		if (right.find("||") == string::npos || exp.find("&&") < exp.find("||"))
			right = right.substr(0, right.find("&&"));
		else if (right.find("||") != string::npos)
			right = right.substr(0, right.find("||"));

		if (CheckLogicalCondition(nextSeparator, left, right, sessionKey))
		{
			exp.erase(0, exp.find(nextSeparator) + 2);
			exp.insert(0, "true");
		}
		else
		{
			exp.erase(0, exp.find(nextSeparator) + 2);
			exp.insert(0, "false");
		}
	}*/

	// Singleton expression
	if (exp.find("&&") == string::npos && exp.find("||") == string::npos)
	{
		return CheckSingletonExpression(exp, sessionKey);
	}
	// Two-element expression
	else if (exp.find("&&") == string::npos || exp.find("||") == string::npos)
	{
		// Check for duplicates
		int pos = 0, ANDs = 0, ORs = 0;
		while (true)
		{
			if (exp.find("&&", pos) != string::npos)
			{
				ANDs += 1;
				pos = exp.find("&&", pos) + 2;
			}
			else
				break;
		}
		pos = 0;
		while (true)
		{
			if (exp.find("||", pos) != string::npos)
			{
				ORs += 1;
				pos = exp.find("||", pos) + 2;
			}
			else
				break;
		}
		if (ANDs == 1 && ORs == 0)
		{
			string left = exp.substr(0, exp.find("&&"));
			string right = exp.substr(exp.find("&&") + 2);
			return CheckLogicalCondition("&&", left, right, sessionKey);
		}
		if (ANDs == 0 && ORs == 1)
		{
			string left = exp.substr(0, exp.find("||"));
			string right = exp.substr(exp.find("||") + 2);
			return CheckLogicalCondition("||", left, right, sessionKey);
		}
	}
	else
	{
		util.cout("Multiple conditions not yet supported", 10, "Error");
		return false;
	}
	return false;
}

bool cRPGNetwork::CheckLogicalCondition(string type, string left, string right, string sessionKey)
{
	bool leftResult = CheckSingletonExpression(left, sessionKey);
	bool rightResult = CheckSingletonExpression(right, sessionKey);
	// Condition
	if (type == "&&")
		return (leftResult && rightResult);
	else if (type == "||")
		return (leftResult || rightResult);
	else
		return false;
}

bool cRPGNetwork::CheckSingletonExpression(std::string exp, std::string sessionKey)
{
	if (exp == "true")
		return true;
	if (exp == "false")
		return false;

	string type, key, value;
	if (exp.find("==") != string::npos)
		type = "==";
	else if (exp.find("!=") != string::npos)
		type = "!=";
	key = exp.substr(0, exp.find(type));
	value = exp.substr(exp.find(type) + 2);

	return CheckComparisonCondition(type, key, value, sessionKey);
}

bool cRPGNetwork::CheckComparisonCondition(string type, string key, string value, string sessionKey)
{
	string valueOfKey = "";
	cRPGClient client = GetClientDataBySession(sessionKey);
	cRPGWorldData world = parent->GetWorldData();
	if (client.sessionKey != "missingno")
	{
		if (key == "race")
			valueOfKey = client.characterRace;
		else if (key == "gender")
			valueOfKey = client.characterGender;
		else if (key == "time")
			valueOfKey = world.Time;

		if (type == "==")
			return (value == valueOfKey);
		else if (type == "!=")
			return (value != valueOfKey);
	}
	return false;
}

string cRPGNetwork::CreateLocationResponse(string locationName, string sessionKey)
{
	string response = "";
	if (locationName != "")
	{
		cRPGLocation locationHandle = parent->FindLocation(locationName);
		if (locationHandle.guid != "missingno")
		{
			response += "guid=" + locationHandle.guid + "%%";
			response += "description=" + ParseLocationText(locationHandle.text, sessionKey) + "%%";
			response += "decCount=" + to_string((int)locationHandle.decision.size()) + "%%";
			for (int i = 0; i < (int)locationHandle.decision.size(); i++)
			{
				response += locationHandle.decision[i].text;
				if (i < (int)locationHandle.decision.size() - 1)
					response += "%%";
			}
		}
	}
	return response;
}

string cRPGNetwork::CreateCharacterResponse(string sessionKey, string characterName = "")
{
	string response = "";
	cRPGClient client;
	if (characterName.length() > 0)
		client = GetClientDataByCharacterName(characterName);
	else
		client = GetClientDataBySession(sessionKey);

	if (client.sessionKey != "missingno")
	{
		response += "gender=" + client.characterGender + "%%";
		response += "race=" + client.characterRace + "%%";
	}
	return response;
}

cRPGClient cRPGNetwork::GetClientDataBySession(string sessionKey)
{
	for (int i = 0; i < (int)client.size(); i++)
	{
		if (client[i].sessionKey == sessionKey)
			return client[i];
	}
	return cRPGClient("missingno");
}

cRPGClient cRPGNetwork::GetClientDataByCharacterName(string characterName)
{
	for (int i = 0; i < (int)client.size(); i++)
	{
		if (client[i].playerName == characterName)
			return client[i];
	}
	return cRPGClient("missingno");
}

std::string cRPGNetwork::GenerateSessionKey()
{
	string keyGenerator = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	int keyLength = randomizer.getInt(8, 12);
	string sessionKey = "";
	for (int i = 0; i < keyLength; i++)
	{
		sessionKey += keyGenerator[randomizer.getInt(0, (int)keyGenerator.length() - 1)];
	}
	return sessionKey;
}