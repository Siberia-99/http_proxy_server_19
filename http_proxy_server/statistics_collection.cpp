#include "stdafx.h"
#include "statistics_collection.hpp"
#include "Parcer.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

//string to_string(int n)
//{
//	stringstream ss;
//	ss << n;
//	return ss.str();
//}

statictics_collection::statictics_collection(string request, string response, const char *id)
{
	this->_request = request;
	this->_response = response;
	this->_id = id;
	_request_length = 0;
	_response_length = 0;
}


statictics_collection::~statictics_collection()
{
}

void statictics_collection::statictics_collection_get_data_valume()
{
	
	string st = ("users/" + string(_id)+".txt");
	cout << st.c_str();
	HANDLE HF = CreateFile(_T(st.c_str()), GENERIC_READ | GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (HF == INVALID_HANDLE_VALUE) {
		cout << "can not craeteFile statistiv" << GetLastError() << endl;
	}
	
	

	cout <<endl<< "We in statatic cpp" << _request_length << " : " << _response_length << endl;
	string buf_to_file = _T("id user: " + (string)_id + "\nVolume data to server: " + to_string(_request_length) + "\nVolume data from server: " + to_string(_response_length));

	cout << buf_to_file.c_str() << endl;
	DWORD nByte = 1;
	
	WriteFile(HF, buf_to_file.c_str(), sizeof(char)*buf_to_file.length(), &nByte, NULL);
	
	//int recv= 
	cout << "recv: " << nByte;


	

}