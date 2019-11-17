#pragma once
#include "stdafx.h"
#include <string>
#include <regex>
#include <iostream>
#include <WinSock.h>
using namespace std;


class return_cash
{
public:
	return_cash();
	~return_cash();
	string return_file_from_directory(string _fileName);
	bool check_in_cash(string _host);
	HANDLE return_handle(string _host);
private:

};

