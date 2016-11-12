
#pragma once

#include "stdafx.h"

class cDish
{
public:
	int Id;
	std::string Name;
	std::string Url;
	std::string Date;

	std::string Data;

	cDish(int Id, std::string Date);
};

class cClientMain
{
public:
	bool TestUserLoggedIn = false;
	std::vector<cDish> TestUserDishHistory;

	bool IsLoggedIn(std::string username);
	int Login(std::string username, std::string passwordHash);
	int Logout(std::string sessionKey);
	std::vector<cDish> GetUserDishHistory(std::string sessionKey);

	void SeedData();
};

extern cClientMain Client;