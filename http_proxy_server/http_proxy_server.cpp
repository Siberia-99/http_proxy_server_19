#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include "socket_wrap.hpp"
#include "socket_wrap_server.hpp"
#include "socket_wrap_client.hpp"
#include "Parser.hpp"
#include "parcer_request.hpp"

#pragma comment(lib,"Ws2_32.lib")
#include <WinSock.h>

#define ip "192.168.1.24"
#define port 8080
using namespace std;
#define request "GET /index.html HTTP/1.0 \r\nHost: www.Shelek.com\r\n\r\n" // HTML запрос.
#define MAX_PACKET_SIZE 16096

int runTCPClient(SOCKET _client_socket,  char* _buf_request, const char * addr)
{
	//создаю объект класса для работы с сокетами общего назначения
	socket_wrap A;

	//инициирование использования библиотеки DLL Winsock процессом
	A.initialization_WinSock();

	//Создание сокета для связи  с запрашиваемым сервером
	SOCKET server_socket = A.create_socket();

	//создаю объект класса для работы с сокетами для сервера
	socket_wrap_server B;


	//ip адрес не всегда верный!!!????
	//Получение структуры, содержащий IP запрашиваемого хоста 
	hostent* hn = B.return_IP_requested_host(addr);


	//Создание структуры для соединения с сервером
	sockaddr_in adr;
	adr.sin_family = AF_INET;
	adr.sin_port = htons(80);
	adr.sin_addr.S_un.S_addr = *(DWORD*)hn->h_addr_list[0];
	
	cout << "connect with: " << addr << endl;
	cout << "Ip address connect with: " << inet_ntoa(adr.sin_addr)<<":"<<ntohs(adr.sin_port)<<endl;

	//Устанавливаю соединение  с ервером
	cout << "result connect: " << B.connect_with_server(server_socket, (sockaddr*)&adr, sizeof(adr)) << endl;
	
	
	//////ЧТО ЗА СУПЕР ПРЕОБРАЗОВАНИЯ&&&&&&&???
	//send(server_socket, request, sizeof(request), 0);
	//Посылаем запрос
	send(server_socket, /*(char*)&*/_buf_request, MAX_PACKET_SIZE, 0);
	//A.send_message(server_socket, /*(char*)&*/_buf_request, sizeof(_buf_request), 0);
	//ссылка?

	cout << "we send message!" << endl;

	//Создание массива для принятия сообщения с сервера
	char buff_response[MAX_PACKET_SIZE];


	// Получаем ответ
	int len = A.recv_message(server_socket, (char *)&buff_response, MAX_PACKET_SIZE, 0);
	//int len = recv(server_socket, (char *)&buff_response, MAX_PACKET_SIZE, 0);
	/*int len;
	do
	{
		if (SOCKET_ERROR == (len = recv(server_socket, (char *)&buff_response, MAX_PACKET_SIZE, 0)))
			return -1;
		for (int i = 0; i < len; i++)
			printf("%c", buff_response[i]);
	} while (len != 0);*/



	
	
	// Выводим результат
	for (int i = 0; i < len; ++i)
		cout << buff_response[i];

	cout << "we recv message!" << endl;
	cout << endl;
	

	A.send_message(_client_socket, buff_response, MAX_PACKET_SIZE, 0);

	// Закрываем соединение
	//A.close_socket(server_socket);
	
	cout << "end";
	Sleep(1000);
	return 1;
}


int runTCPClient2(char* _buf_request, const char * addr)
{
	WSADATA ws;
	SOCKET s;
	sockaddr_in adr;
	hostent* hn;
	char buff[MAX_PACKET_SIZE];
	// Init 
	if (WSAStartup(0x0101, &ws) != 0)
	{
		// Error
		return -1;
	}
	// Создаём сокет
	if (INVALID_SOCKET == (s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
	{
		// Error
		return -1;
	}
	
	// Получаем адрес 
	if (NULL == (hn = gethostbyname(addr)))
	{
		// Error
		return -1;
	}

	// Заполняем структуру с адресом
	adr.sin_family = AF_INET;
	adr.sin_addr.S_un.S_addr = *(DWORD*)hn->h_addr_list[0];
	adr.sin_port = htons(80);

	cout << "connect with: " << addr << endl;
	cout << "Ip address connect with: " << inet_ntoa(adr.sin_addr) << endl;
	// Устанавливаем соединение с сервером
	if (SOCKET_ERROR == connect(s, (sockaddr*)&adr, sizeof(adr)))
	{
		// Error
		return -1;
	}
	
	// Посылаем запрос
	if (SOCKET_ERROR == send(s, (char*)&_buf_request, sizeof(_buf_request), 0))
	{
		// Error
		return -1;
	}

	// Получаем ответ
	int len = recv(s, (char *)&buff, MAX_PACKET_SIZE, 0);

	if ((len == SOCKET_ERROR) || (len == 0))
	{
		// Error
		return -1;
	}

	// Выводим результат
	for (int i = 0; i < len;++i)
		printf("%c", buff[i]);

		// Закрываем соединение
		if (SOCKET_ERROR == closesocket(s))
		{
			// Error
			return -1;
		}

	// Всё! :)
	return 1;
}

int handler(SOCKET &_socket, sockaddr_in _TSockAddr_new)
{
	socket_wrap A;
	A.initialization_WinSock();

	while (1)
	{
		cout << "handler" << endl;
		char buf_request[MAX_PACKET_SIZE] = "";

		int actual_len = A.recv_message(_socket, buf_request, MAX_PACKET_SIZE, 0);




		if (actual_len == 0)
		{
			cout << "all" << endl;
			return 7;
		}

		cout << actual_len << endl;


		cout << "///////////////////////////////////////////////////////////Request from client///////////////////////////////////////////////////////////" << endl;
		//for (int i = 0;i < actual_len; ++i)
		//	cout << (buf_request[i]);
		cout << buf_request << endl;
		cout << "///////////////////////////////////////////////////////////Request from client///////////////////////////////////////////////////////////" << endl;

		cout << "Message lenthg: " << actual_len << "Client connected: " << inet_ntoa(_TSockAddr_new.sin_addr) << " --" << ntohs(_TSockAddr_new.sin_port) << endl; Sleep(3000);

		Parser pars; pars.parse_request((string)buf_request); string host_str = pars.host;

		const char *host = host_str.data();

		///////////////////SERVER//////////////////////////////
		SOCKET server_socket = A.create_socket();
		socket_wrap_server B;
		hostent* hn = B.return_IP_requested_host(host);
		cout << hn;
		//Создание структуры для соединения с сервером
		sockaddr_in adr;
		adr.sin_family = AF_INET;
		adr.sin_port = htons(80);/////был 80
		adr.sin_addr.S_un.S_addr = *(DWORD*)hn->h_addr_list[0];

		cout << "connect with: " << host << endl;
		cout << "Ip address connect with: " << inet_ntoa(adr.sin_addr) << ":" << ntohs(adr.sin_port) << endl;

		//Устанавливаю соединение  с cервером
		cout << "result connect: " << B.connect_with_server(server_socket, (sockaddr*)&adr, sizeof(adr)) << endl;

		//setsockopt()
		/////////сообщение серверу
		send(server_socket, buf_request, MAX_PACKET_SIZE, 0);

		cout << "we send message!" << endl;
		char buff_response[MAX_PACKET_SIZE] = "";

		//int len = A.recv_message(server_socket, (char *)&buff_response, MAX_PACKET_SIZE, 0);

		int len = recv(server_socket, (char *)&buff_response, MAX_PACKET_SIZE, 0);
		//while(len!=0)
		//recv(server_socket, (char *)&buff_response, MAX_PACKET_SIZE, 0);
		/*int len;
		do
		{
		if (SOCKET_ERROR == (len = recv(server_socket, (char *)&buff_response, MAX_PACKET_SIZE, 0)))
		return -1;
		for (int i = 0; i < len; ++i)
		printf("%c", buff_response[i]);
		} while (len != 0);*/


		cout << "///////////////////////////////////////////////////////////Response from server///////////////////////////////////////////////////////////" << endl;
		//for (int i = 0;i < len; ++i)
		//	cout << (buff_response[i]);
		cout << buff_response;
		cout << "///////////////////////////////////////////////////////////Response from server///////////////////////////////////////////////////////////" << endl;

		/*
		char *buf_response_change = new char[len];
		for (int i = 0;i < actual_len; ++i) {
		buf_response_change[i] = buff_response[i];
		}*/


		cout << "we recv message!" << len << endl;
		cout << endl;


		//A.send_message(_socket, buff_response, MAX_PACKET_SIZE, 0);
		send(_socket, buff_response, MAX_PACKET_SIZE, 0);

		


		/*
		while (1)
		{
		char buf_request[MAX_PACKET_SIZE] = "";

		//int actual_len = 0;
		int actual_len = A.recv_message(_socket, buf_request, MAX_PACKET_SIZE, 0);

		send(server_socket, buf_request, MAX_PACKET_SIZE, 0);
		char buff_response[MAX_PACKET_SIZE] = "";
		int len = A.recv_message(server_socket, (char *)&buff_response, MAX_PACKET_SIZE, 0);
		A.send_message(_socket, buff_response, MAX_PACKET_SIZE, 0);

		}*/
		// Закрываем соединение
		//A.close_socket(server_socket);

		cout << "end";
		Sleep(1000);
		//}
		
		closesocket(server_socket);
		closesocket(_socket);
	}
	return 1;

}

int receiver(unsigned short _port, const char *_ip_aadr)
{
	socket_wrap A;
	A.initialization_WinSock();

	SOCKET listen_socket = A.create_socket();

	cout << "listener socket" << listen_socket << endl;


	//netstat -aon | more
	// структура SockAddr, содержащая данные, необходимые для подключения 
	//(протокол, адрес удаленного компьютера, порт).

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

	//ПЕРЕВОЖУ СОКЕТ С СЛУШАЮЩИЙ РЕЖИМ
	if (SOCKET_ERROR == listen(listen_socket, 4))
		cout << "ERROR WITH listen";

	cout << "Ip_address: " << inet_ntoa(TSockAddr.sin_addr) << " Server is listening to port " << _port << ", waiting for connections... " << endl;


	//структура для подключившегося сокета
	sockaddr_in TSockAddr_new;

	socket_wrap_client C;
	int sizet = sizeof(TSockAddr_new);
	//жду соединения
	SOCKET _socket = C.accept_message(listen_socket, (SOCKADDR*)&TSockAddr_new, &sizet);

	A.send_message(_socket, "Hello, world! start work with sockets!", MAX_PACKET_SIZE, 0);
	int z  = handler(_socket, TSockAddr_new);
	//////////////////////////////////////////////////////////////////////////////////////////
	
		
	closesocket(listen_socket);
	return 1;
}



/*
int runTcpServer(unsigned short _port, const char *_ip_aadr)
{
	socket_wrap A;
	A.initialization_WinSock();
	
	SOCKET listen_socket = A.create_socket();
	
	cout << "listener socket" << listen_socket << endl;


	//netstat -aon | more
	// структура SockAddr, содержащая данные, необходимые для подключения 
	//(протокол, адрес удаленного компьютера, порт).

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

	//ПЕРЕВОЖУ СОКЕТ С СЛУШАЮЩИЙ РЕЖИМ
	listen(listen_socket, 4);

	cout << "Ip_address: " << inet_ntoa(TSockAddr.sin_addr) << " Server is listening to port " << _port << ", waiting for connections... " << endl;

	
	//структура для подключившегося сокета
	sockaddr_in TSockAddr_new;

	socket_wrap_client C;
	
	//жду соединения
	int actual_len = 1;
	SOCKET sock = C.accept_message(listen_socket, (SOCKADDR*)&TSockAddr_new, NULL);
	
	cout << "We in cicle" << endl;
	//Полученнный запрос
	char buf_request[MAX_PACKET_SIZE];

	//int actual_len = 0;
	actual_len = A.recv_message(sock, buf_request, MAX_PACKET_SIZE, 0);

	cout << "///////////////////////////////////////////////////////////Request from client///////////////////////////////////////////////////////////" << endl;
	for (int i = 0;i < actual_len; ++i)
		cout << (buf_request[i]);
	cout << "///////////////////////////////////////////////////////////Request from client///////////////////////////////////////////////////////////" << endl;

	cout << actual_len << "Client connected: " << inet_ntoa(TSockAddr_new.sin_addr) << " --" << ntohs(TSockAddr_new.sin_port) << endl;
	Sleep(3000);


	Parser PARSE_request;

	PARSE_request.parse_request((string)buf_request);

	//cout <<"parse"<< PARSE_request.host.c_str()<<endl;
	string host = PARSE_request.host;
	const char *char_host = host.data();


	for (int i = 0; i < PARSE_request.host.length(); ++i)
		cout << PARSE_request.host.c_str()[i];

	cout << endl;
	runTCPClient(sock, buf_request, PARSE_request.host.c_str());
	

	closesocket(sock);
	return 1;
}
*/

int main()
{
	//runTcpServer(8080, "172.20.10.2");
	//runTCPClient("www.machinelearning.ru");
	//runTCPClient("www.machinelearning.ru");
	//#define request "GET /index.html HTTP/1.0 \r\nHost: www.Shelek.com\r\n\r\n" 
	char buf[] = "GET /index.html HTTP/1.0 \r\nHost: www.Shelek.com\r\n\r\n";
	const char *addr = "www.Shelek.com";
	//runTCPClient2(request, "www.Shelek.com");
	//cout << buf;
	receiver(8080, "172.20.10.2");

	/*string a = "Hello";
	a.data();
	for (int i = 0; i < size(a); ++i)
		cout << a[i];*/
	//runTCPClient(NULL, buf, addr);
	//hostent a =  MYreturnIP("www.Shelek.com");
	//cout << inet_ntoa(a.h_addr_list[0]);
	/*string a = "GET http://www.shelec.com ru";
	Parser A;
	A.parse_request(a);
	cout << A.host.c_str();*/
    return 0;
}

