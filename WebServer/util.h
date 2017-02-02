
#include "stdafx.h"
#include <locale>
#include <codecvt>

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
	std::wstring s2ws(const std::string& str);
	std::string ws2s(const std::wstring& wstr);
	std::wstring getline(std::wifstream* file, wchar_t* buffer, const int bufferSize);

	std::mutex coutAccess;

	cSettings settings;
};

class cRandom
{
public:
	std::mt19937 engine;

	void seed();
	// Get a random integer from interval [min; max]
	int getInt(int min, int max);
	// Get a random double from interval [min; max]
	double getDouble(double min, double max);
};

extern cUtilMain util;
extern cRandom randomizer;