
#include "stdafx.h"
#include "json.h"
#include "client.h"

cJsonMain Json;

std::string cJsonMain::Assemble_DishHistory(std::string sessionKey)
{
	std::string json = "[";

	std::vector<cDish> DishHistory = Client.GetUserDishHistory(sessionKey);
	for (int i = 0; i < (int)DishHistory.size(); i++)
	{
		if (i > 0)
			json += ",";
		json += "{";

		// Id
		json += "\"id\":";
		json += "\"" + std::to_string(DishHistory[i].Id) + "\"";
		// Date
		json += ",\"date\":";
		json += "\"" + DishHistory[i].Date + "\"";
		// Data
		json += ",\"data\":";
		json += "" + DishHistory[i].Data + "";

		json += "}";
	}

	json = json.append("]");
	return json;
}