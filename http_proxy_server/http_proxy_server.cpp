#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include "socket_wrap.hpp"
#include "socket_wrap_server.hpp"
#include "socket_wrap_client.hpp"
#include "Parcer.hpp"
#include "parcer_request.hpp"
#include <thread>
#include "return_cash.hpp"
#pragma comment(lib,"Ws2_32.lib")
#include <WinSock.h>
#define use_cash 1

using namespace std;
#define MAX_PACKET_SIZE 5000
//#define BUFF_SIZE;

//Обработчик заявок
void WINAPI handler(SOCKET _socket)
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	//-----------------------------------------CLIENT----------------------------------------------------//

	///////////////////////////////////////////////////////////////////////////////////////////////////////



	socket_wrap A;
	A.initialization_WinSock();
	cout << "Handler create!!! " << "id thread : " << this_thread::get_id()<< endl;
	
	//принимаю запрос от браузера
	char buf_request[MAX_PACKET_SIZE];
	int request_length = A.recv_message(_socket, buf_request, MAX_PACKET_SIZE, 0);
	string client_request = (string)buf_request;
	cout <<"request_length: "<< request_length << endl;

	cout << "//////////////////////////////////////////////////////////0/Request from client///////////////////////////////////////////////////////////" << endl;
	for (int i = 0; i<request_length; ++i)
		cout << buf_request[i];
	cout << "//////////////////////////////////////////////////////////1/Request from client///////////////////////////////////////////////////////////" << endl;

	//парсинг запроса и выделение хоста
	parcer pars; 
	pars.parse_request(client_request); 
	string host_str = pars.host;

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	//-----------------------------------------CASH----------------------------------------------------//

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	if (use_cash == 1)
	{
		return_cash Abc;
		HANDLE HF = Abc.return_handle(host_str);
		if (HF!= INVALID_HANDLE_VALUE) {
			cout << endl << endl << "USE CASH" << endl << endl << endl << endl << endl;
			TCHAR BufferRead[MAX_PACKET_SIZE];
			

			cout << "My Lovely" << HF << endl;
			////////какого не работает??????????????????
			//string ste = "lol/" + host_str;
			//cout << ste.data() << ":"<<endl;
			//if (ste == "lol/iantonov.me") cout << "qew";
			//HANDLE HF = CreateFile(_T(ste.data()), GENERIC_READ, 0, NULL,
			//	OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			//if (HF == INVALID_HANDLE_VALUE) {
			//	printf("ERROR with ReadFile(cash)%x \n", GetLastError());

			//}
			////////////////////////////

			//HANDLE HF = Abc.return_handle(host_str);

			DWORD lpNumberOfBytesRead = 0;
			while (ReadFile(HF, &BufferRead, MAX_PACKET_SIZE, &lpNumberOfBytesRead, NULL) && lpNumberOfBytesRead > 0) {
				int c = send(_socket, BufferRead, lpNumberOfBytesRead, 0);
				cout << "Length to browser: " << c << endl;
			}

			ExitThread(87);
		}

	}

	string directory_cash = _T("lol/" + host_str);

	//Создал файл для записи сайта
	HANDLE crf = CreateFile(directory_cash.data(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (crf == INVALID_HANDLE_VALUE) {
		printf("ERROR with CreateFile for cash%x \n", GetLastError());

	}
	//////////////////////////////////////////////////////////////CASH////////////////////////////////

	cout << "parse host: " << host_str << endl;
	/*if (host_str == "CONNECT www.google.com:443 HTTP" || host_str == "CONNECT mc.yandex.ru:443 HTTP")
	{
		cout << "error with host 443 HTTP!!! -->ExitThread" << endl;
		ExitThread(9);
	}*/
	const char *host = host_str.data();

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	//-----------------------------------------SERVER----------------------------------------------------//

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	//создание сокета для работы с сервером и передача ему ip адреса сервера и порта
	SOCKET server_socket = A.create_socket();
	socket_wrap_server B;
	hostent* hn = B.return_IP_requested_host(host);
	if (hn == NULL)
	{
		cout << "NO NAME HOST -->ExitThread";
		ExitThread(1);
	}

	sockaddr_in adr;
	adr.sin_family = AF_INET;
	adr.sin_port = htons(80);
	adr.sin_addr.S_un.S_addr = *(DWORD*)hn->h_addr_list[0];

	cout << "connect with: " << host << endl;
	cout << "Ip address connect with: " << inet_ntoa(adr.sin_addr) << ":" << ntohs(adr.sin_port) << endl;
	cout << "result connect: " << B.connect_with_server(server_socket, (sockaddr*)&adr, sizeof(adr)) << endl;

	//отправка запроса от клиента серверу
	int b = send(server_socket, buf_request, request_length, 0);

	cout << "we send message to server! Length: " <<b<< endl;
	
	//Создание массива для принятия ответа от севера
	char buff_response[MAX_PACKET_SIZE];
	
	//http://iantonov.me/page/proxy-server-na-delphi-i-winsock-api
	//https://club.shelek.ru/viewart.php?id=35
	
	//время ожидания ответа от сервера
	int timeout = 20000;
	if (setsockopt(server_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(DWORD)))
	{
		perror("setsockopt");
		//return -1;
	}


	

	int response_length = 1;
	while (response_length != 0 || response_length!= -1)
	{
		response_length = A.recv_message(server_socket, buff_response, MAX_PACKET_SIZE, 0);
		if (response_length == -1)
			break;
		cout << "==="<<response_length<<"======================================================================" << endl;
		cout << "///////////////////////////////////////////////////////////Response from server///////////////////////////////////////////////////////////" << endl;
		for (int i = 0; i<response_length; ++i)
		cout << buff_response[i];
		cout << "///////////////////////////////////////////////////////////Response from server///////////////////////////////////////////////////////////" << endl;
		

		//Паранойя
		//if (use_cash == 0)
		WriteFile(crf, buff_response, MAX_PACKET_SIZE, 0, NULL);
		
		
		int c = send(_socket, buff_response, response_length, 0);
		cout << "Length to browser: " << c << endl;
		memset(buff_response, 0, MAX_PACKET_SIZE);
	}


	cout <<endl<< "-------------------------------------handler close--------------------------------------" << endl << endl;
	
	Sleep(4000);
	closesocket(server_socket);
	closesocket(_socket);
}

//Функция для прослушивания сокета и создания обработчика для каждого клиента
int receiver(unsigned short _port, const char *_ip_aadr)
{
	//Инициализация работы с сокетами
	socket_wrap A;
	A.initialization_WinSock();

	SOCKET listen_socket = A.create_socket();

	cout << "listener socket: " << listen_socket << endl;


	//netstat -aon | more
	// структура SockAddr, содержащая данные, необходимые для подключения 
	sockaddr_in TSockAddr;
	TSockAddr.sin_family = AF_INET;//семейство протоколов
	TSockAddr.sin_port = htons(_port);//порт, с которым нужно будет установить соединение
	TSockAddr.sin_addr.s_addr = inet_addr(_ip_aadr); //структура, в которой записана информация об адресе удаленного компьютера //то так называемый подстановочный адрес IPv4. Подстановочный IP-адрес полезен для приложений, которые связывают сокеты доменов Интернета на многосетевых хостах.
	//inet_addr() преобразует обычный вид IP-адреса cp (из номеров и точек) в двоичный код в сетевом порядке расположения байтов.
	//htons преобразовывает короткое целое из формата локального хоста в сетевой формат
	//ntons обратная верхней
	//inet_ntoa преобразует 32-разрядный двоичный адрес IPv4, хранящийся в сетевом порядке байтов, в точечно-десятичную строку.

	//name.sin_addr.S_un.S_addr = htonl (INADDR_ANY);
	//name.sin_port = htons(0);
	//Для того, что бы предоставить системе право выбора IP и порта адреса мы должны указать INADDR_ANY в качестве IP - адреса, и 0 для порта.


	//связывание структуры TSockAddr с созданным сокетом. 
	A.bind_struct_with_socket(listen_socket, reinterpret_cast<SOCKADDR*>(&TSockAddr), sizeof(TSockAddr));

	//перевожу сокет в слушающий режим
	if (SOCKET_ERROR == listen(listen_socket, 4))
		cout << "ERROR WITH listen";
	
	cout << "Ip_address: " << inet_ntoa(TSockAddr.sin_addr) << " Server is listening to port " << _port << ", waiting for connections... id thread: " << this_thread::get_id() << endl;


	vector<char*> list_of_users;
	//Жду запросов)
	while (1)
	{
		//структура для подключившегося сокета
		sockaddr_in TSockAddr_new;

		socket_wrap_client C;
		int sizet = sizeof(TSockAddr_new);
		
		//жду соединения
		SOCKET _socket = C.accept_message(listen_socket, (SOCKADDR*)&TSockAddr_new, &sizet);
		cout << " Client connected: " << inet_ntoa(TSockAddr_new.sin_addr) << " --" << ntohs(TSockAddr_new.sin_port) << endl; 

		list_of_users.push_back(inet_ntoa(TSockAddr_new.sin_addr));

		cout << "Vector: " << list_of_users[0] << endl;

		//Для каждого подключившегося
		DWORD pidThread = 0;
		
		HANDLE h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)handler, (LPVOID)_socket, 0, &pidThread);
		
		
		
		
		//WaitForSingleObject(h, INFINITE);
		//cout << "handler close" << endl;
	}
	
	closesocket(listen_socket);
	return 1;
}



int main()
{
	//return_cash A;
	//cout << A.check_in_cash("iantonov.me") << endl;


	/*HANDLE HF = CreateFile(_T("lol/iantonov.me"), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (HF == INVALID_HANDLE_VALUE) {
		printf("ERROR with ReadFile(cash)%x \n", GetLastError());

	}
	cout<<"OK";*/
	receiver(8080, "172.20.10.3");
	
	//return_cash A;
	//cout << A.check_in_cash("www.tadviser.ru");
	//A.return_file_from_directory("www.tadviser.ru");

	/*
	HANDLE crf = CreateFile(_T("X64/hello"), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (crf == INVALID_HANDLE_VALUE) {
		printf("ERROR with ReadFile%x \n", GetLastError());
		return 2;
	}

	string str = "hello, lox";
	const char *buf = "Hello, world!";
	//buf  = _T("Hello, world!");
	cout << sizeof(buf)*sizeof(buf[0]);
	
	DWORD nNumberOfWritten = 0;
	if (!WriteFile(crf, str.data(), sizeof(str), &nNumberOfWritten, NULL)) cout<<GetLastError();
	cout << "n:" <<nNumberOfWritten;
	*/

    return 0;
}

