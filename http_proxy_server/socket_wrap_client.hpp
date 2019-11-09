#pragma once
#include "stdafx.h"
#include "windows.h"
#include <WinSock.h>
#include <iostream>
using namespace std;


class socket_wrap_client
{
public:
	socket_wrap_client();
	~socket_wrap_client();
	SOCKET accept_message(SOCKET _socket,struct sockaddr *_addr,int *_addrlen);
};


