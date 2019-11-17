#include "stdafx.h"
#include "socket_wrap_server.hpp"

socket_wrap_server::socket_wrap_server()
{
}
socket_wrap_server::~socket_wrap_server()
{
}

hostent * socket_wrap_server::return_IP_requested_host(const char * _host)
{
	hostent* hn;
	if (NULL == (hn = gethostbyname(_host)))
	{
		cout << "Error in gethostbyname: " << WSAGetLastError() << endl;
	}
	return hn;
}

int socket_wrap_server::connect_with_server(SOCKET _socket, const struct sockaddr *_name, int _namelen)
{
	if (SOCKET_ERROR == connect(_socket, _name, _namelen))
	{
		cout << "Error in connect: " << WSAGetLastError() << endl;
		
		return -1;
	}
	return 0;
}