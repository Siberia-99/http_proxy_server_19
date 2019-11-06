#pragma once
#include "stdafx.h"
#include "windows.h"
#include <WinSock.h>
#include <iostream>
using namespace std;

class Parser
{
public:
	int type;
	string protocol;
	string host;
	string path;
	Parser();
	~Parser();
	void parse_request(string _request);
};

