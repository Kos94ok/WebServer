
#include "stdafx.h"
#include "rpg_client.h"

using namespace std;

cRPGClient::cRPGClient(string sessionKey)
{
	this->sessionKey = sessionKey;
}

void cRPGClient::MoveToLocation(string location)
{
	this->currentLocation = location;
}