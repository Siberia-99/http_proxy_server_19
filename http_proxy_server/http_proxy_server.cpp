#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include "socket_wrap.hpp"
#include "socket_wrap_server.hpp"
#include "socket_wrap_client.hpp"
#include "Parser.hpp"


#pragma comment(lib,"Ws2_32.lib")
#include <WinSock.h>

#define ip "192.168.1.24"
#define port 8080
using namespace std;
#define request "GET /index.html HTTP/1.0 \r\nHost: www.Shelek.com\r\n\r\n" // HTML запрос.
#define MAX_PACKET_SIZE 4096

int runTCPClient(SOCKET _client_socket,  const char* _buf_request, const char * addr)
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
	cout << "Ip address connect with: " << inet_ntoa(adr.sin_addr)<<endl;

	//Устанавливаю соединение  с ервером
	cout << "result connect: " << B.connect_with_server(server_socket, (sockaddr*)&adr, sizeof(adr)) << endl;
	
	
	//send(server_socket, request, sizeof(request), 0);
	//Посылаем запрос
	A.send_message(server_socket, _buf_request, sizeof(_buf_request), 0);
	//ссылка?

	cout << "we send message" << endl;

	//Создание массива для принятия сообщения с сервера
	char buff_response[MAX_PACKET_SIZE];


	// Получаем ответ
	//int len = A.recv_message(server_socket, (char *)&buff_response, MAX_PACKET_SIZE, 0);
	//int len = recv(server_socket, (char *)&buff_response, MAX_PACKET_SIZE, 0);
	int len;
	do
	{
		if (SOCKET_ERROR == (len = recv(server_socket, (char *)&buff_response, MAX_PACKET_SIZE, 0)))
			return -1;
		for (int i = 0; i < len; i++)
			printf("%c", buff_response[i]);
	} while (len != 0);



	
	cout << "we recv message" << endl;
	// Выводим результат
	for (int i = 0; i < len; ++i)
		cout << buff_response[i];

	// Закрываем соединение
	if (SOCKET_ERROR == closesocket(server_socket))
	{
		// Error
		return -1;
	}

	A.send_message(_client_socket, buff_response, sizeof(buff_response), 0);

	// Закрываем соединение
	A.close_socket(server_socket);

	cout << "end";
	Sleep(1000);
	return 1;
}




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
	SOCKET sock = C.accept_message(listen_socket, (SOCKADDR*)&TSockAddr_new, NULL);
	
	//Полученнный запрос
	char buf_request[MAX_PACKET_SIZE];

	int actual_len = 0;
	actual_len = A.recv_message(sock, buf_request, MAX_PACKET_SIZE, 0);
	
	cout << "///////////////////////////////////////////////////////////Request from client///////////////////////////////////////////////////////////"<<endl;
	for (int i = 0;i < actual_len; ++i)
		cout<< (buf_request[i]);
	cout << "///////////////////////////////////////////////////////////Request from client///////////////////////////////////////////////////////////" << endl;

	cout << actual_len <<"Client connected: " << inet_ntoa(TSockAddr_new.sin_addr) <<" --" <<ntohs(TSockAddr_new.sin_port)<< endl;
	Sleep(3000);


	Parser PARSE_request;

	PARSE_request.parse_request((string)buf_request);

	//cout <<"parse"<< PARSE_request.host.c_str()<<endl;
	string str = PARSE_request.host;

	

	for (int i = 0; i < PARSE_request.host.length(); ++i)
		cout <<PARSE_request.host.c_str()[i];
	
	cout << endl;
	runTCPClient(sock, buf_request, PARSE_request.host.c_str());


	
	return 1;
}


int main()
{
	runTcpServer(8080, "172.20.10.2");
	//runTCPClient("www.machinelearning.ru");
	//runTCPClient("www.machinelearning.ru");
	//hostent a =  MYreturnIP("www.Shelek.com");
	//cout << inet_ntoa(a.h_addr_list[0]);
	/*string a = "GET http://www.shelec.com ru";
	Parser A;
	A.parse_request(a);
	cout << A.host.c_str();*/
    return 0;
}

