
#include "stdafx.h"

class cUtilMain
{
public:
	void initDecoder();
	void addDecoderEntry(string in, string out);
	string decodeString(string str);
	string getTimeStr();
};

extern cUtilMain util;