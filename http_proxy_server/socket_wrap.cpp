#include "stdafx.h"
#include "socket_wrap.hpp"

socket_wrap::socket_wrap()
{
}

socket_wrap::~socket_wrap()
{
}

int socket_wrap::initialization_WinSock()
{
	WSADATA wsaData;

	//WSAStartup ���������� ������������� ���������� DLL Winsock ���������.
	//� ������ ������, �� ������� WSAStartup ���������� ����
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		cout << "Error WSAStartup: " << result << WSAGetLastError() << endl;
		return 1;
	}

	return 0;
}

SOCKET socket_wrap::create_socket()
{
	//�������� ������
	//����������� ���������� ����� ����� �����.
	//af � ��������� ����������. ��� ����������� ���� TCP, ������� ����� ��������� AF_INET.
	//type � ��� ������������ ������. ����� ���� sock_stream (��� ��������� TCP/IP) 
	//protocol � ��������. ��� TCP ����� ������� IPPROTO_TCP
	SOCKET _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket == INVALID_SOCKET)
	{
		cout << "Error socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}

	return(_socket);
}

int socket_wrap::send_message(SOCKET _socket, const char*_buf, int _len, int _flags)
{
	//��� ���� ��� �� ������� ������ ���������� ������� send
	if (SOCKET_ERROR == send(_socket, _buf, _len,_flags))
	{
		cout << "Error in send: " << WSAGetLastError() << endl;
		return -1;
	}
	return 0;
}

int socket_wrap::recv_message(SOCKET _socket, char *_buf, int _len, int _flags)
{
	int len;
	if (SOCKET_ERROR == (len = recv(_socket, _buf, _len, _flags)))
	{
		cout << "Error in recv: " << WSAGetLastError() << endl;
		return -1;
	}
		
	return len;
}

int socket_wrap::close_socket(SOCKET _socket)
{
	if (SOCKET_ERROR == closesocket(_socket))
	{
		return -1;
	}
	return 0;
}

int socket_wrap::bind_struct_with_socket(SOCKET _socket, const struct sockaddr *_addr, int _namelen)
{
	if (bind(_socket, _addr, _namelen) == SOCKET_ERROR)
	{
		cout << "Error in BIND(): " << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}
	return 0;
}