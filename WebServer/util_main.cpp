
#include "stdafx.h"
#include "util.h"
#include "ejb.h"

using namespace std;

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

void cUtilMain::cout(string str, int level, int threadId)
{
	if (settings.enableCout == 0 || level < settings.outputLevel) { return; }
	// Add thread id
	if (threadId != -1) {
		str = "[" + to_string(threadId) + "] " + str;
	}
	// Add timestamp
	str = getTimeStr() + str;
	// Flush data
	coutAccess.lock();

	std::cout << str << "\n";

	coutAccess.unlock();
}

void cSettings::load()
{
	string val = getEntry("enableCout");
	stringstream(val) >> enableCout;
	val = getEntry("enableProxy");
	stringstream(val) >> enableProxy;
	val = getEntry("outputLevel");
	stringstream(val) >> outputLevel;
}

string cSettings::getEntry(string name)
{
	string retval, parse;
	ifstream file("settings.ini");
	if (file.good())
	{
		while (!file.eof())
		{
			getline(file, parse);
			if (parse.substr(0, parse.find(" = ")) == name) {
				retval = parse.substr(parse.find(" = ") + 3, parse.length() - parse.find(" = ") - 3);
				break;
			}
		}
	}
	file.close();
	return retval;
}