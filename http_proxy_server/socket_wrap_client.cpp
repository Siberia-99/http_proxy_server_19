#include "stdafx.h"
#include "socket_wrap_client.hpp"


socket_wrap_client::socket_wrap_client()
{
}

socket_wrap_client::~socket_wrap_client()
{
}

SOCKET socket_wrap_client::accept_message(SOCKET _socket, struct sockaddr FAR *_addr, int FAR *_addrlen)
{
	return(accept(_socket, _addr, _addrlen));
}