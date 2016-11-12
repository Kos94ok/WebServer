
#include "stdafx.h"

class cSettings
{
public:
	int enableLog = 1;
	int enableCout = 1;
	int enableProxy = 0;
	int outputLevel = 16;
	int consoleWidth = 80;
	int mainPort = 8080;
	std::string mainPath = "Website/";

	void load();
	std::string getEntry(std::string name);
};

class cUtilMain
{
public:
	void initDecoder();
	void addDecoderEntry(std::string in, std::string out);
	std::string decodeString(std::string str);
	std::string getTimeStr();
	void cout(std::string str, int level = 8, std::string prefix = "", int threadId = -1);
	int parseArguments(std::string args, std::string* key, std::string* value);
	std::string getArgumentValue(std::string target, std::string* key, std::string* value, int count = 8);
	std::string executeSystemCall(std::string cmd);

	std::mutex coutAccess;

	cSettings settings;
};

extern cUtilMain util;