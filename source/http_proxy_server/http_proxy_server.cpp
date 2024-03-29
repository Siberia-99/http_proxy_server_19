#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include "socket_wrap.hpp"
#include "socket_wrap_server.hpp"
#include "socket_wrap_client.hpp"
#include "Parser.hpp"
#include "parcer_request.hpp"
#include <thread>
#pragma comment(lib,"Ws2_32.lib")
#include <WinSock.h>

using namespace std;
#define MAX_PACKET_SIZE 5000


//Обработчик заявок
void WINAPI handler(SOCKET _socket)
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	//-----------------------------------------CLIENT----------------------------------------------------//

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	SOCKET server_socket;
	socket_wrap A;
	A.initialization_WinSock();
	cout << "Handler create!!! " << "id thread : " << this_thread::get_id()<< endl;
	while (true)
	{


		//принимаю запрос от браузера
		char buf_request[MAX_PACKET_SIZE];
		memset(buf_request, 0, MAX_PACKET_SIZE);
		int request_length = A.recv_message(_socket, buf_request, MAX_PACKET_SIZE, 0);
		string client_request = (string)buf_request;
		cout << "request_length: " << request_length << endl;
		if (request_length == 0) break;
		cout << "//////////////////////////////////////////////////////////0/Request from client///////////////////////////////////////////////////////////" << endl;
		for (int i = 0; i < request_length; ++i)
			cout << buf_request[i];
		cout << "//////////////////////////////////////////////////////////1/Request from client///////////////////////////////////////////////////////////" << endl;

		//парсинг запроса и выделение хоста
		Parser pars;
		pars.parse_request(client_request);
		string host_str = pars.host;
		cout << "parse host: " << host_str << endl;
		if (host_str == "CONNECT mc.yandex.ru:443 HTTP" || host_str == "CONNECT www.google.com:443 HTTP") {
			cout << "error with host 443" << endl;
			ExitThread(7);
		}
		const char *host = host_str.data();

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		//-----------------------------------------SERVER----------------------------------------------------//

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		//создание сокета для работы с сервером и передача ему ip адреса сервера и порта
		/*SOCKET */server_socket = A.create_socket();
		socket_wrap_server B;
		hostent* hn = B.return_IP_requested_host(host);
		if (hn == NULL) break;
		sockaddr_in adr;
		adr.sin_family = AF_INET;
		adr.sin_port = htons(80);
		adr.sin_addr.S_un.S_addr = *(DWORD*)hn->h_addr_list[0];

		cout << "connect with: " << host << endl;
		cout << "Ip address connect with: " << inet_ntoa(adr.sin_addr) << ":" << ntohs(adr.sin_port) << endl;
		cout << "result connect: " << B.connect_with_server(server_socket, (sockaddr*)&adr, sizeof(adr)) << endl;

		//отправка запроса от клиента серверу
		int b = send(server_socket, buf_request, request_length, 0);

		cout << "we send message to server! Length: " << b << endl;

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
		while (response_length != 0 /*|| response_length != -1*/)
		{
			response_length = A.recv_message(server_socket, buff_response, MAX_PACKET_SIZE, 0);
			if (response_length == -1)
				break;
			cout << "===" << response_length << "======================================================================" << endl;
			cout << "///////////////////////////////////////////////////////////Response from server///////////////////////////////////////////////////////////" << endl;
			for (int i = 0; i < response_length; ++i)
				cout << buff_response[i];
			cout << "///////////////////////////////////////////////////////////Response from server///////////////////////////////////////////////////////////" << endl;

			int c = send(_socket, buff_response, response_length, 0);
			cout << "Length to browser: " << c << endl;
			memset(buff_response, 0, MAX_PACKET_SIZE);
		}


		cout << endl << "-------------------------------------handler close--------------------------------------" << endl << endl;

		Sleep(4000);
	}
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

	//Жду запросов)
	while (1)
	{
		
		//структура для подключившегося сокета
		sockaddr_in TSockAddr_new;

		socket_wrap_client C;
		int sizet = sizeof(TSockAddr_new);
		
		//жду соединения
		SOCKET _socket = C.accept_message(listen_socket, (SOCKADDR*)&TSockAddr_new, &sizet);
		cout << "+++++++++++++++++++++++++++++++++++++++++++++++new+++++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << " Client connected: " << inet_ntoa(TSockAddr_new.sin_addr) << ": " << ntohs(TSockAddr_new.sin_port) << endl; 

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
	receiver(8080, "172.20.10.2");

    return 0;
}

