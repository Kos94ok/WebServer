
#include "stdafx.h"
#include "util.h"

cUtilMain util;

string cUtilMain::decodeString(string str)
{
	string val;
	while (str.find("%3A") != string::npos || str.find("%2F") != string::npos || str.find("%21") != string::npos)
	{
		val = str.substr(str.find("%"), 3);
		if (val == "%3A") { str.replace(str.find("%"), 3, ":"); }
		else if (val == "%2F") { str.replace(str.find("%"), 3, "/"); }
		else if (val == "%21") { str.replace(str.find("%"), 3, " "); }
	}
	return str;
}

string cUtilMain::getTimeStr()
{
	string val = "";
	time_t t = time(0);
	struct tm now;
	localtime_s(&now, &t);

	if (now.tm_hour < 10) { val += "0"; }
	val += to_string(now.tm_hour) + ":";
	if (now.tm_min < 10) { val += "0"; }
	val += to_string(now.tm_min) + ":";
	if (now.tm_sec < 10) { val += "0"; }
	val += to_string(now.tm_sec);
	val += " ";
	return val;
}