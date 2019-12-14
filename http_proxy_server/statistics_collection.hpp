#pragma once
#include "stdafx.h"
#include "windows.h"
#include <WinSock.h>
#include <iostream>
using namespace std;


class statictics_collection
{
public:
	string _request; 
	string _response;
	const char *_id;
	SOCKET _socket;
	int _request_length;
	int _response_length;
	void add_request(int n) { _request_length += n; };
	void add_response(int n) { _response_length += n; };
	statictics_collection(string request, string response, const char *id);
	~statictics_collection();
	void statictics_collection_get_data_valume();

private:

};

