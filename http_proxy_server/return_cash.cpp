#include "stdafx.h"
#include "return_cash.hpp"
#define BUFF 32
return_cash::return_cash()
{
}

return_cash::~return_cash()
{
}

bool return_cash::check_in_cash(string _host)
{
	string st = ("lol/" + _host);

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
	string st = ("lol/" + _host);

	HANDLE HF = CreateFile(_T(st.data()), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (HF == INVALID_HANDLE_VALUE) {
		printf("File not exist (cash)%x \n", GetLastError());
		Sleep(5000);
		return FALSE;
	}

	return HF;
}