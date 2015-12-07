
#include "stdafx.h"

class cEJBMain
{
public:
	string from(string filename, string entry);
	int fromI(string filename, string entry);
	void push(string filename, string key, string value);

	string parse(string script);
};

extern cEJBMain ejb;