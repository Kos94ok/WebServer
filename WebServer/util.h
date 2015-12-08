
#include "stdafx.h"

class cSettings
{
public:
	int enableLog = 1;
	int enableCout = 1;
	int enableProxy = 0;
	int outputLevel = 16;
	int consoleWidth = 80;
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

	std::mutex coutAccess;

	cSettings settings;
};

extern cUtilMain util;