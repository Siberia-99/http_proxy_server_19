#pragma once
#include "stdafx.h"
#include "windows.h"
#include <WinSock.h>
#include <iostream>
using namespace std;

class socket_wrap
{
public:
	socket_wrap();
	~socket_wrap();
	int initialization_WinSock();
	SOCKET create_socket();
	int send_message(SOCKET _socket, const char*_buf, int _len, int _flags);
	int recv_message(SOCKET _socket, char *_buf, int _len, int _flags);
	int close_socket(SOCKET _socket);
	int bind_struct_with_socket(SOCKET _socket, const struct sockaddr *_addr, int _namelen);
};

