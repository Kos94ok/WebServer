// stdafx.h: ���������� ���� ��� ����������� ��������� ���������� ������
// ��� ���������� ������ ��� ����������� �������, ������� ����� ������������, ��
// �� ����� ����������
//

#pragma once
#pragma comment(lib, "Ws2_32.lib")

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
#endif

//#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

extern string mainpath;

string ejb_from(string filename, string entry);
int ejb_fromI(string filename, string entry);
void ejb_push(string filename, string key, string value);
string parseEJB(string script);
void sendPage(string url, SOCKET* client);
void sendData(string data, SOCKET* client);
string getTimeStr();
string decodeURL(string str);