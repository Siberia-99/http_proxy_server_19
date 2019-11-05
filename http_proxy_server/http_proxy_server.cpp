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

struct hostent MYreturnIP(const char *addr)
{

	WSADATA wsaData;

	//Этот Функция WSAStartup инициирует использование библиотеки DLL Winsock процессом.
	//В случае успеха, то Функция WSAStartup возвращает ноль
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		cout << "Error WSAStartup: " << result << WSAGetLastError() << endl;
		//return;
	}

	//адресу www.Shelek.com / index.html.
	//Для того, что бы узнать IP адрес машины зная ёё имя gethostbyname.
	//Для получения имени машины по ёё адресу используем функцию gethostbyaddr.
	
	hostent* d_addr; // Структура, в которую будет помещен IP адрес, // при возврате.
	memset(&d_addr, 0, sizeof(d_addr));
	d_addr = gethostbyname(addr);
	if (d_addr == NULL)
		cout << "lox" << WSAGetLastError();
	//cout <<"++"<< hn->h_name << "++" << hn->h_addr_list[1] << "++" << hn->h_addrtype << "++" <<hn->h_aliases;
	
	return *d_addr;
}

int runTCPClient(SOCKET _client_socket, char * _buf, const char * addr)
{
	//создаю объект класса для работы с сокетами общего назначения
	socket_wrap A;

	//инициирование использования библиотеки DLL Winsock процессом
	A.initialization_WinSock();

	//Создание сокета для связи  с запрашиваемым сервером
	SOCKET sock = A.create_socket();

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
	B.connect_with_server(sock, (sockaddr*)&adr, sizeof(adr));
	
	//Посылаем запрос
	A.send_message(sock, _buf, sizeof(_buf), 0);

	cout << "we send message" << endl;

	//Создание массива для принятия сообщения с сервера
	char buff[MAX_PACKET_SIZE];


	// Получаем ответ
	//int len = A.recv_message(sock, (char *)&buff, MAX_PACKET_SIZE, 0);
	int len = recv(sock, (char *)&buff, MAX_PACKET_SIZE, 0);
	
	
	cout << "we recv message" << endl;
	// Выводим результат
	for (int i = 0; i < len; ++i)
		cout << buff[i];

	A.send_message(_client_socket, buff, sizeof(buff), 0);

	// Закрываем соединение
	A.close_socket(sock);

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
	char buf[MAX_PACKET_SIZE];

	int actual_len = 0;
	actual_len = A.recv_message(sock, buf, MAX_PACKET_SIZE, 0);
	
	cout << "///////////////////////////////////////////////////////////Request from client///////////////////////////////////////////////////////////"<<endl;
	for (int i = 0;i < actual_len; ++i)
		cout<< (buf[i]);
	cout << "///////////////////////////////////////////////////////////Request from client///////////////////////////////////////////////////////////" << endl;

	cout << actual_len <<"Client connected: " << inet_ntoa(TSockAddr_new.sin_addr) <<" --" <<ntohs(TSockAddr_new.sin_port)<< endl;
	Sleep(3000);


	Parser PARSE_request;

	PARSE_request.parse_request((string)buf);

	//cout <<"parse"<< PARSE_request.host.c_str()<<endl;
	string str = PARSE_request.host;

	

	for (int i = 0; i < PARSE_request.host.length(); ++i)
		cout <<PARSE_request.host.c_str()[i];
	cout << endl;
	runTCPClient(sock, buf, PARSE_request.host.c_str());


	
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

