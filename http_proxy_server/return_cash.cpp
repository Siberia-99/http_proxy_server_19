#include "stdafx.h"
#include "return_cash.hpp"
#define BUFF 32
#define MAX_PACKET_SIZE 5000
return_cash::return_cash()
{
}

return_cash::~return_cash()
{
}

bool return_cash::check_in_cash(string _host)
{
	string st = ("cash/" + _host);

	HANDLE HF = CreateFile(_T(st.data()), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (HF == INVALID_HANDLE_VALUE) {
		printf("File not exist %x \n", GetLastError());
		Sleep(5000);
		return FALSE;
	}
	
	return TRUE;
}

HANDLE return_cash::return_handle(string _host) {
	string st = ("cash/" + _host);

	HANDLE HF = CreateFile(_T(st.data()), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (HF == INVALID_HANDLE_VALUE) {
		printf("File not exist (cash)%x \n", GetLastError());
		Sleep(5000);
		return INVALID_HANDLE_VALUE;
	}

	return HF;
}


BOOL return_cash::use_cash(HANDLE _readFile, SOCKET &_clientSocket)
{

	//если 
	if (_readFile != INVALID_HANDLE_VALUE) {
		cout << endl << endl << "We use Cash" << endl << endl;

		//Массив для отправки ответа серверу(Кэш считываем)
		TCHAR BufferRead[MAX_PACKET_SIZE];
		memset(BufferRead, 0, MAX_PACKET_SIZE);

		//Считываем с файла и отправляем серверу
		DWORD lpNumberOfBytesRead = 0;
		while (ReadFile(_readFile, &BufferRead, MAX_PACKET_SIZE, &lpNumberOfBytesRead, NULL) && lpNumberOfBytesRead > 0) {


			
			//cout << "///////////////////////////////////////////////////////////Response from server cash///////////////////////////////////////////////////////////" << endl;
			//for (int i = 0; i < lpNumberOfBytesRead; ++i)
			//	cout << BufferRead[i];
			//cout << "///////////////////////////////////////////////////////////Response from server cash///////////////////////////////////////////////////////////" << endl;


			int c = send(_clientSocket, BufferRead, lpNumberOfBytesRead, 0);

			cout << "Length to browser in cash: " << lpNumberOfBytesRead <<"Really send: " << c << endl;
			if (c == -1) cout << "Error in recv to server in cash" << GetLastError() << endl;
		}
		return TRUE;
	}
	else return FALSE;
}