
#include "stdafx.h"
#include "util.h"

cUtilMain util;

vector<string> dictIn;
vector<string> dictOut;

void cUtilMain::addDecoderEntry(string in, string out)
{
	dictIn.push_back(in);
	dictOut.push_back(out);
}

void cUtilMain::initDecoder()
{
	addDecoderEntry("%3A", ":");
	addDecoderEntry("%2F", "/");
	addDecoderEntry("%21", " ");
	addDecoderEntry("%2C", ",");
	addDecoderEntry("+", " ");
	addDecoderEntry("%0D%0A", "%01");
}

string cUtilMain::decodeString(string str)
{
	bool repeat = true;
	while (repeat) {
		repeat = false;
		for (int i = 0; i < (int)dictIn.size(); i++) {
			if (str.find(dictIn[i]) != string::npos) {
				str.replace(str.find(dictIn[i]), dictIn[i].length(), dictOut[i]);
				repeat = true;
			}
		}
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