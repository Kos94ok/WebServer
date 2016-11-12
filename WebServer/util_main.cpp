
#include "stdafx.h"
#include "util.h"
#include <cstdio>
#include <memory>
#include <stdexcept>

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
	addDecoderEntry("%3C", "<");
	addDecoderEntry("%3E", ">");
	addDecoderEntry("%92", "\\");
	addDecoderEntry("%28", "(");
	addDecoderEntry("%29", ")");
	addDecoderEntry("%96", "-");
	addDecoderEntry("%26", "&");
	addDecoderEntry("%27", "'");
	addDecoderEntry("%22", "\"");
	addDecoderEntry("%3B", ";");
	addDecoderEntry("+", " ");
	addDecoderEntry("%0D%0A", "<br>");
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

	// Milliseconds
	val += ".";
	int ms = timeGetTime() % 1000;
	if (ms < 10) { val += "0"; }
	if (ms < 100) { val += "0"; }
	val += to_string(ms);

	// Return
	val += " ";
	return val;
}

void cUtilMain::cout(string str, int level, string prefix, int threadId)
{
	if (settings.enableLog == 0 && (settings.enableCout == 0 || level < settings.outputLevel)) { return; }
	// Add prefix
	if (prefix != "") {
		str = prefix + "] " + str;
		if (threadId == -1) {
			str = "[" + str;
		}
	}
	// Add thread id
	if (threadId != -1) {
		if (prefix == "") {
			str = "[" + to_string(threadId) + "] " + str;
		}
		else {
			str = "[" + to_string(threadId) + "/" + str;
		}
	}
	// Add timestamp
	str = getTimeStr() + str;

	// Flush to file
	if (settings.enableLog == 1) {
		ofstream file("log.txt", ios::app);
		file << str << "\n";
		file.close();
	}

	if (settings.enableCout == 0 || level < settings.outputLevel) { return; }

	// Check the length
	if (settings.consoleWidth < 1000 && (int)str.length() > settings.consoleWidth - 1) {
		str = str.substr(0, settings.consoleWidth - 4) + "...";
	}
	// Flush data
	coutAccess.lock();

	std::cout << str << "\n";

	coutAccess.unlock();
}

int cUtilMain::parseArguments(std::string args, std::string* key, std::string* value)
{
	using namespace std;

	int found = 0;
	while (true)
	{
		key[found] = args.substr(0, args.find("="));
		if (args.find("&") != string::npos) {
			if (args.find("&") != args.find("=") + 1) {
				value[found] = args.substr(args.find("=") + 1, args.find("&") - args.find("=") - 1);
			}
			else {
				value[found] = "";
			}
			args.erase(0, args.find("&") + 1);
			found += 1;
		}
		else {
			value[found] = args.substr(args.find("=") + 1);
			found += 1;
			break;
		}
	}
	return found;
}

string cUtilMain::getArgumentValue(std::string target, std::string* key, std::string* value, int count)
{
	for (int i = 0; i < count; i++)
	{
		if (key[i] == target)
			return value[i];
	}
	return "";
}

string cUtilMain::executeSystemCall(string cmd)
{
	char buffer[4096];
	std::string result = "";
	std::shared_ptr<FILE> pipe(_popen(cmd.c_str(), "r"), _pclose);
	if (!pipe) throw std::runtime_error("popen() failed!");
	while (!feof(pipe.get())) {
		if (fgets(buffer, 128, pipe.get()) != NULL)
			result += buffer;
	}
	return result;
}

void cSettings::load()
{
	string val = getEntry("enableLog");
	stringstream(val) >> enableLog;
	val = getEntry("enableCout");
	stringstream(val) >> enableCout;
	val = getEntry("enableProxy");
	stringstream(val) >> enableProxy;
	val = getEntry("outputLevel");
	stringstream(val) >> outputLevel;
	val = getEntry("consoleWidth");
	stringstream(val) >> consoleWidth;
	val = getEntry("mainPath");
	stringstream(val) >> mainPath;
	val = getEntry("mainPort");
	stringstream(val) >> mainPort;
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