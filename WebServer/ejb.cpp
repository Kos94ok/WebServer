
#include "stdafx.h"

string ejb_from(string filename, string entry)
{
	string retval, parse;
	ifstream file(mainpath + filename);
	file.close();
	file.open(mainpath + filename);
	if (file.good())
	{
		while (!file.eof())
		{
			char buf[1024];
			file.getline(buf, 1024);
			parse = buf;
			cout << parse << endl;
			if (parse.substr(0, parse.find(" = ")) == entry) {
				retval = parse.substr(parse.find(" = ") + 3);
				break;
			}
		}
	}
	file.close();
	return retval;
}

int ejb_fromI(string filename, string entry)
{
	int retval;
	string str = ejb_from(filename, entry);
	stringstream(str) >> retval;
	return retval;
}

void ejb_push(string filename, string key, string value)
{
	string parse;
	string inName = mainpath + filename;
	string outName = mainpath + filename + "_temp";
	ifstream in(inName);
	stringstream tempOut;
	bool added = false;
	int lifeguard = 0;
	if (in.good())
	{
		while (!in.eof() && lifeguard < 1000)
		{
			lifeguard += 1;
			char buf[256];
			in.getline(buf, 256);
			parse = buf;
			if (parse.substr(0, parse.find(" = ")) == key) {
				added = true;
				tempOut << key << " = " << value << "\r\n";
			}
			else {
				tempOut << parse;
			}
		}
	}
	if (!added) {
		tempOut << key << " = " << value << "\r\n";
	}
	DeleteFile(wstring(inName.begin(), inName.end()).c_str());
	ofstream out(inName);
	out << tempOut.str();
}

string parseEJB(string script)
{
	string retval = "", parse;
	string cmd = script.substr(0, script.find(":"));
	string args = script.substr(script.find(":") + 2);
	if (cmd == "from")
	{
		string filename = args.substr(0, args.find(" "));
		string entry = args.substr(args.find(" ") + 1);
		ejb_from(filename, entry);
	}
	else if (cmd == "flush")
	{
		string filename = args.substr(0, args.find(" "));
		ifstream file(mainpath + filename);
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