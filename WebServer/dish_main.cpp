
#include "stdafx.h"
#include "client.h"
#include "util.h"

cDish::cDish(int Id)
{
	this->Id = Id;
	this->Name = "Test Name";  
	this->Url = "http://mealvation.fi/hackthemeal/display/getdish.php?dishid=" + std::to_string(Id);

	this->Data = util.executeSystemCall("curl.exe -s " + this->Url);
	this->Data = this->Data.substr(0, this->Data.length() - 1);
}