#pragma once
#include "stdafx.h"
#include "windows.h"
#include <WinSock.h>
#include <iostream>
using namespace std;

class parcer
{
public:
	int type;
	string protocol;
	string host;
	string path;
	parcer();
	~parcer();
	void parse_request(string _request);
};

