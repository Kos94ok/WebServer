
#include "stdafx.h"

class cEJBMain
{
public:
	std::string from(std::string filename, std::string entry);
	int fromI(std::string filename, std::string entry);
	void push(std::string filename, std::string key, std::string value);

	std::string parse(std::string script);
};

extern cEJBMain ejb;