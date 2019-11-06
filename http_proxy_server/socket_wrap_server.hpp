#pragma once
#include "stdafx.h"
#include "windows.h"
#include <WinSock.h>
#include <iostream>
using namespace std;

class socket_wrap_server
{
public:
	socket_wrap_server();
	~socket_wrap_server();
	hostent* return_IP_requested_host(const char * _host);
	int connect_with_server(SOCKET _socket,const struct sockaddr *_name, int _namelen);
	
};
