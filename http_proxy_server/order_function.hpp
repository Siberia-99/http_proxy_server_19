#pragma once
#include "stdafx.h"
#include "windows.h"
#include <WinSock.h>
#include <iostream>
using namespace std;


//Класс, содержащий функции, призванные упростить код, однако серьезной смысловй или функциональной нагрузки не несущие
class order_function
{
public:
	sockaddr_in _TSockAddr;
	order_function();
	~order_function();
	BOOL set_information_about_new_user(HANDLE &writeFile, sockaddr_in TSockAddr_new);
	void EnterMessage(char *buf, int length, string Name);
	sockaddr_in set_struct_sockaddr_in(unsigned short port, const char *ip);
	HANDLE write_to_file(const char * name);
private:

};

