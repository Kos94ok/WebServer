
#include "stdafx.h"
#include "ejb.h"
#include "util.h"

using namespace std;

cEJBMain ejb;

string cEJBMain::from(string filename, string entry)
{
	string retval, parse;
	ifstream file(util.settings.mainPath + filename);
	if (file.good())
	{
		while (!file.eof())
		{
			getline(file, parse);
			if (parse.substr(0, parse.find(" = ")) == entry) {
				retval = parse.substr(parse.find(" = ") + 3, parse.length() - parse.find(" = ") - 3);
				break;
			}
		}
	}
	file.close();
	return retval;
}

int cEJBMain::fromI(string filename, string entry)
{
	int retval;
	string str = from(filename, entry);
	stringstream(str) >> retval;
	return retval;
}

void cEJBMain::push(string filename, string key, string value)
{
	string parse;
	string inName = util.settings.mainPath + filename;
	ifstream in(inName);
	string tempOut[256];
	int stringsOut = 0;
	bool added = false;
	int lifeguard = 0;
	if (in.good())
	{
		while (!in.eof() && lifeguard < 1000)
		{
			lifeguard += 1;
			char buf[65536];
			in.getline(buf, 65536);
			parse = buf;
			if (parse.substr(0, parse.find(" = ")) == key) {
				added = true;
				tempOut[stringsOut++] = key + " = " + value;
			}
			else {
				tempOut[stringsOut++] = parse;
			}
		}
	}
	if (!added) {
		tempOut[stringsOut++] = key + " = " + value;
	}
	DeleteFile(wstring(inName.begin(), inName.end()).c_str());
	ofstream out(inName);
	for (int i = 0; i < stringsOut; i++) {
		if (tempOut[i].length() > 0) {
			out << tempOut[i] << endl;
		}
	}
}

string cEJBMain::parse(string script)
{
	string retval = "", parse;
	string cmd = script.substr(0, script.find(":"));
	string args = script.substr(script.find(":") + 2);
	if (cmd == "from")
	{
		string filename = args.substr(0, args.find(" "));
		string entry = args.substr(args.find(" ") + 1);
		retval = from(filename, entry);
	}
	else if (cmd == "flush")
	{
		string filename = args.substr(0, args.find(" "));
		ifstream file(util.settings.mainPath + filename);
		if (file.good())
		{
			ostringstream oss;
			oss << file.rdbuf();
			retval = oss.str();
		}
		file.close();
	}
	return retval;
}