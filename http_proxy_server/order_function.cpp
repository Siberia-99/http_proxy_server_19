#include "stdafx.h"
#include "order_function.hpp"
#include <mutex>
#include <time.h>


mutex mtx1;
order_function::order_function()
{
	
}

order_function::~order_function()
{
}

//������� ���������� � �������������� ������������ � ���������� ��������������� ������������ � ����
BOOL order_function::set_information_about_new_user(HANDLE &writeFile, sockaddr_in _TSockAddr)
{
	mtx1.lock();
	cout << "_____________________________________new user________________________________" << endl;
	cout << "Client connected: " << inet_ntoa(_TSockAddr.sin_addr) << " : " << ntohs(_TSockAddr.sin_port) << endl;
	mtx1.unlock();

	
	//������� ��������� � �������������� ������������ � ����
	string buf_to_file = _T((string)inet_ntoa(_TSockAddr.sin_addr) + "\n");
	WriteFile(writeFile, buf_to_file.c_str(), sizeof(char)*buf_to_file.length(), 0, NULL);
	return TRUE;
}


//������� ������ buf ����� length � ������ Name
void order_function::EnterMessage(char *buf, int length, string Name)
{
	mtx1.lock();
	cout << "//////////////////////////////////////////////////////////0/Request from " << Name.c_str() <<"///////////////////////////////////////////////////////////" << endl;
	for (int i = 0; i < length; ++i)
		cout << buf[i];
	cout << "//////////////////////////////////////////////////////////1/Request from " << Name.c_str() << "///////////////////////////////////////////////////////////" << endl;
	mtx1.unlock();

}



sockaddr_in order_function::set_struct_sockaddr_in(unsigned short port, const char *ip)
{
	sockaddr_in TSockAddr;
	TSockAddr.sin_family = AF_INET;//��������� ����������
	TSockAddr.sin_port = htons(port);//����, � ������� ����� ����� ���������� ����������
	TSockAddr.sin_addr.s_addr = inet_addr(ip); //���������, � ������� �������� ���������� �� ������ ���������� ���������� //�� ��� ���������� �������������� ����� IPv4. �������������� IP-����� ������� ��� ����������, ������� ��������� ������ ������� ��������� �� ������������ ������.
	//inet_addr() ����������� ������� ��� IP-������ cp (�� ������� � �����) � �������� ��� � ������� ������� ������������ ������.
	//htons ��������������� �������� ����� �� ������� ���������� ����� � ������� ������
	//ntons �������� �������
	//inet_ntoa ����������� 32-��������� �������� ����� IPv4, ���������� � ������� ������� ������, � �������-���������� ������.
	return TSockAddr;
}

HANDLE order_function::write_to_file(const char * name)
{
	HANDLE File_for_write_cash = CreateFile(_T(name), GENERIC_READ | GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (File_for_write_cash == INVALID_HANDLE_VALUE) printf("ERROR with CreateFile for cash%x \n", GetLastError());
	return File_for_write_cash;
}