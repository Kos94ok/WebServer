
#include "stdafx.h"
#include "rpg.h"
#include "util.h"
#include "socket.h"
#include <codecvt>

using namespace std;

cRPG rpg;

void cRPG::Initialization()
{
	db.parent = this;
	network.parent = this;
	ParseDatabase();
}