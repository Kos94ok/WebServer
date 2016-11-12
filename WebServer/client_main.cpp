
#include "stdafx.h"
#include "client.h"

cClientMain Client;

bool cClientMain::IsLoggedIn(std::string username)
{
	return TestUserLoggedIn;
}


int cClientMain::Login(std::string username, std::string passwordHash)
{
	// Return codes:
	// 0 - success
	// 1 - generic error
	// 2 - argument error
	// 3 - incorrect user credentials

	TestUserLoggedIn = true;

	return 0;
}

int cClientMain::Logout(std::string sessionKey)
{
	TestUserLoggedIn = false;

	return 0;
}

std::vector<cDish> cClientMain::GetUserDishHistory(std::string sessionKey)
{
	return std::vector<cDish>(TestUserDishHistory);
}

void cClientMain::SeedData()
{
	TestUserDishHistory.push_back(cDish(1, "08.11.2016"));
	TestUserDishHistory.push_back(cDish(2, "09.11.2016"));
	TestUserDishHistory.push_back(cDish(5, "10.11.2016"));
}