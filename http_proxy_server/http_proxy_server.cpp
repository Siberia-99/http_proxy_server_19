#include "stdafx.h"
#include "windows.h"
#include <iostream>

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


int runTcpServer(unsigned short _port, const char *_ip_aadr)
{

	WSADATA wsaData;

	//Этот Функция WSAStartup инициирует использование библиотеки DLL Winsock процессом.
	//В случае успеха, то Функция WSAStartup возвращает ноль
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		cout << "Error WSAStartup: " << result << WSAGetLastError() << endl;
		return 1;
	}
//
//	//Создание сокета
//	//Результатом выполнения будет новый сокет.
//	//af – семейство протоколов. Нам потребуется лишь TCP, поэтому будем указывать AF_INET.
//	//type – тип создаваемого сокета. Может быть sock_stream (для протокола TCP/IP) 
//	//protocol – протокол. Для TCP нужно указать IPPROTO_TCP
//	SOCKET listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (listen_socket == INVALID_SOCKET)
//	{
//		cout << "Error socket(): " << WSAGetLastError() << endl;
//		WSACleanup();
//		return 1;
//	}
//	cout << "listener socket" << listen_socket << endl;
//
//
//	//netstat -aon | more
//	// структура SockAddr, содержащая данные, необходимые для подключения 
//	//(протокол, адрес удаленного компьютера, порт).
//
//	sockaddr_in TSockAddr;
//	TSockAddr.sin_family = AF_INET;//семейство протоколов
//	TSockAddr.sin_port = htons(_port);//порт, с которым нужно будет установить соединение
//	TSockAddr.sin_addr.s_addr = inet_addr(_ip_aadr); //inet_addr(ip);//INADDR_LOOPBACK= inet_addr(ip);//htonl(INADDR_ANY);//структура, в которой записана информация об адресе удаленного компьютера //то так называемый подстановочный адрес IPv4. Подстановочный IP-адрес полезен для приложений, которые связывают сокеты доменов Интернета на многосетевых хостах.
//	//inet_addr() преобразует обычный вид IP-адреса cp (из номеров и точек) в двоичный код в сетевом порядке расположения байтов.
//	//htons преобразовывает короткое целое из формата локального хоста в сетевой формат
//	//inet_ntoa преобразует 32-разрядный двоичный адрес IPv4, хранящийся в сетевом порядке байтов, в точечно-десятичную строку.

//	//name.sin_addr.S_un.S_addr = htonl (INADDR_ANY);
//	//name.sin_port = htons(0);
//	//Для того, что бы предоставить системе право выбора IP и порта адреса мы должны указать INADDR_ANY в качестве IP - адреса, и 0 для порта.


//
//	//Назначение функции – связывание структуры TSockAddr с созданным сокетом. 
//	int bindOUR = bind(listen_socket, reinterpret_cast<SOCKADDR*>(&TSockAddr), sizeof(TSockAddr));
//	if (bindOUR == SOCKET_ERROR)
//	{
//		cout << "Error in BIND(): " << WSAGetLastError() << endl;
//		WSACleanup();
//		return 1;
//	}
//	//ПЕРЕВОЖУ СОКЕТ С СЛУШАЮЩИЙ РЕЖИМ
//	listen(listen_socket, 4);
//	cout << "Ip_address: " << inet_ntoa(TSockAddr.sin_addr) << " Server is listening to port " << _port << ", waiting for connections... " << endl;
//
//
//
//
//	//структура для подключившегося сокета
//	sockaddr_in TSockAddr_new;
//
//	//жду соединения
//	
//	SOCKET sock = accept(listen_socket, (SOCKADDR*)&TSockAddr_new, NULL);
//	char buf[1024];
//
//	int actual_len = 0;
//	actual_len = recv(sock, buf, 1024, 0);
//	if (actual_len == SOCKET_ERROR)
//	{
//		cout << "Error in RECV: " << WSAGetLastError() << endl;
//	}
//
//	for (int i = 0;i < actual_len; ++i)
//		cout<< (buf[i]);
//	//cout << "buf" << buf << endl;
//	cout << actual_len <<"Client connected: " << inet_ntoa(TSockAddr_new.sin_addr) <<" --" <<ntohs(TSockAddr_new.sin_port)<< endl;
//	Sleep(3000);

	//////////////////delete https://club.shelek.ru/
	SOCKET sock;
	if (INVALID_SOCKET == (sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
	{
		cout << "Error in socket: " << WSAGetLastError() << endl;
		return -1;
	}
	///////////////////////////


	cout << "connect with shelec" << endl;

	//Работаем с удаленнным сервером
	hostent* hn;
	if (NULL == (hn = gethostbyname("club.shelek.ru/viewart.php")))
	{
		cout << "Error in gethostbyname: " << WSAGetLastError() << endl;
		return -1;
	}

	//
	sockaddr_in adrnew2;
	adrnew2.sin_family = AF_INET;
	adrnew2.sin_port = htons(80);
	adrnew2.sin_addr.S_un.S_addr = *(DWORD*)hn->h_addr_list[0];


	// Устанавливаем соединение с сервером
	if (SOCKET_ERROR == connect(sock, (sockaddr*)&adrnew2, sizeof(adrnew2)))
	{
		cout << "Error in connect: " << WSAGetLastError() << endl;
		// Error
		return -1;
	}

	// Посылаем запрос
	if (SOCKET_ERROR == send(sock, request, sizeof(request), 0))
	{
		cout << "Error in send: " << WSAGetLastError() << endl;
		// Error
		return -1;
	}
	char buff[MAX_PACKET_SIZE];
	// Получаем ответ
	//int len = recv(sock, (char *)&buff, MAX_PACKET_SIZE, 0);
	int len;
	do
	{
		if (SOCKET_ERROR == (len = recv(s, (char *)&buff, MAX_PACKET_SIZE, 0)))
			return -1;
		for (int i = 0; i < len; i++)
			printf("%c", buff[i]);
	} while (len != 0);





	if ((len == SOCKET_ERROR) || (len == 0))
	{
		cout << "Error in connect: " << WSAGetLastError() << endl;
		// Error
		return -1;
	}

	// Выводим результат
	//for (int i = 0; i; printf("%c", buff[i]);
	for (int i = 0; i < len; ++i)
		cout << buff[i];
	//cout << buff;
		// Закрываем соединение
		if (SOCKET_ERROR == closesocket(sock))
		{
			// Error
			return -1;
		}
	cout << "end";
	Sleep(1000);
	closesocket(sock);
	return 1;
}


int main()
{
	runTcpServer(8080, ip);
	//hostent a =  MYreturnIP("www.Shelek.com");
	//cout << inet_ntoa(a.h_addr_list[0]);
    return 0;
}

