#include "stdafx.h"
//
#include "windows.h"
#include <iostream>
#include "socket_wrap.hpp"
#include "socket_wrap_server.hpp"
#include "socket_wrap_client.hpp"
#include "Parcer.hpp"
#include "parcer_request.hpp"
#include <thread>
#include "return_cash.hpp"
#include "statistics_collection.hpp"
#pragma comment(lib,"Ws2_32.lib")
#include <WinSock.h>
#include <gtest.h>
#include "order_function.hpp"
#include <mutex>
#define USE_CASH 1

using namespace std;
#define MAX_PACKET_SIZE 5000
mutex mtx;

//Структура, которая передается в обработчик
struct Params
{
	sockaddr_in TSockAddr;
	SOCKET socket;
};

//Обработчик заявок
void WINAPI handler(Params par)
{

	SOCKET client_socket = par.socket;
	sockaddr_in TSockAddr_new = par.TSockAddr;
	cout << "Handler create!!! " << "id thread : " << this_thread::get_id() << endl;
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	//-----------------------------------------CLIENT----------------------------------------------------//

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	//Работа с БД
	statictics_collection SC("", "", inet_ntoa(TSockAddr_new.sin_addr));
	socket_wrap A;
	order_function Order_functions;
	SOCKET server_socket;

	//прстоянно работаю с клиентом
	while (1)
	{
		//принимаю запрос от браузера
		char buf_request[MAX_PACKET_SIZE];
		memset(buf_request, 0, MAX_PACKET_SIZE);
		int request_length = A.recv_message(client_socket, buf_request, MAX_PACKET_SIZE, 0);

		//Работа со статистикой
		SC._request = (string)buf_request;
		SC.add_request(request_length);

		//string client_request = (string)buf_request;
		cout << "Длина запроса от клиента: " << request_length << endl;
		
		//В случае, если сообщение не поступило, то собираем вещи
		//break или continue??
		if (request_length == 0 || request_length == -1) break;//break;

		//Вывод запроса от клиента
		//Order_functions.EnterMessage(buf_request, request_length, "client");

		//парсинг запроса и выделение хоста
		parcerRequest PR(SC._request);
		PR.getHost();
		string host_str = PR.getInformation().Host;
		cout << "Parsing... Host: " << host_str << endl;

		
		///////////////////////////////////////////////////////////////////////////////////////////////////////

		//-----------------------------------------CASH----------------------------------------------------//

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		//Если пользователь желает использовать Кэш
		BOOL use_cash_OK = FALSE;
		if (USE_CASH == 1)
		{
			//Получаю сайт из кэша
			return_cash My_cash;
			HANDLE raed_cash_file = My_cash.return_handle(host_str);

			use_cash_OK = My_cash.use_cash(raed_cash_file, client_socket);
			cout <<"use cash_ok "<<use_cash_OK<< " Мы обработали с помощью кэша 1 запрос" << endl;
		}
		//Если кэш решили не использовать
		if (USE_CASH == 0 || use_cash_OK == FALSE)
		{
			///////////////////////////////////////////////////////////////////////////////////////////////////////

			//-----------------------------------------SERVER----------------------------------------------------//

			///////////////////////////////////////////////////////////////////////////////////////////////////////

			const char *host = host_str.data();

			//создание сокета для работы с сервером и передача ему ip адреса сервера и порта
			server_socket = A.create_socket();
			socket_wrap_server B;
			hostent* hn = B.return_IP_requested_host(host);
			if (hn == NULL)
			{
				cout << "Хост неизвестен" << endl;
				break;//break;
			}

			//Если не использовал кэш, то записываю сайт в файл
			string st = ("cash/" + host_str);

			HANDLE File_for_write_cash = Order_functions.write_to_file((const char*)st.c_str());
			
			sockaddr_in adr;
			adr.sin_family = AF_INET;
			adr.sin_port = htons(80);
			adr.sin_addr.S_un.S_addr = *(DWORD*)hn->h_addr_list[0];
			cout << "connect with: " << host << endl;
			//Что-то странное здесь происходитююю
			//cout << "___________________________" << SC._id << "__________________" << endl;
			//cout << "Ip address connect with: " << inet_ntoa(adr.sin_addr) << ":" << ntohs(adr.sin_port) << endl;//ЧТО ЗА??
			//cout << "___________________________" << SC._id << "__________________" << endl;
			cout << "result connect: " << B.connect_with_server(server_socket, (sockaddr*)&adr, sizeof(adr)) << endl;
			//cout << "___________________________" << SC._id << "__________________" << endl;
			
			//отправка запроса от клиента серверу
			int b = send(server_socket, buf_request, request_length, 0);
			cout << "Мы отправили запрос от клиента серверу, длина запроса от клиента" << b << endl;

			//Создание массива для принятия ответа от севера
			char buff_response[MAX_PACKET_SIZE];
			//время ожидания ответа от сервера
			int timeout = 10000;
			if (setsockopt(server_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(DWORD)))perror("setsockopt");


			//Принимаем ответЫ(!!!!) от сервера и перенаправляем клиенту
			int response_length = 1;
			while (response_length != 0)
			{
				response_length = A.recv_message(server_socket, buff_response, MAX_PACKET_SIZE, 0);
				if (response_length == -1)
					break;
				cout << response_length << endl;
				Order_functions.EnterMessage(buff_response, response_length, "server");
				
				//Работа со статистикой
				SC._response += (string)buff_response;
				SC.add_response(response_length);
				cout << SC._response_length << "Сумма длин всех ответов от сервера " << endl;

				DWORD nByteForCash = 0;
				//Работа с кэшем
				int wr = WriteFile(File_for_write_cash, buff_response, response_length, &nByteForCash, NULL);
				if (wr == 0) cout << GetLastError() << endl;

				int c = send(client_socket, buff_response, response_length, 0);
				cout << "Length to browser: " << c << endl;
				memset(buff_response, 0, MAX_PACKET_SIZE);
			}
		}
	}

	//Записываю статистику
	cout << SC._response_length << "<--rESPONSE " << endl;
	SC.statictics_collection_get_data_valume();
	SC.~statictics_collection();
	cout <<endl<< "-------------------------------------handler close--------------------------------------" << endl << endl;
	
	Sleep(4000);
	closesocket(client_socket);
}



//Функция для прослушивания сокета и создания обработчика для каждого клиента
int receiver(unsigned short _port, const char *_ip_aadr)
{
	//Инициализация работы с сокетами
	socket_wrap A;
	A.initialization_WinSock();

	//Класс для работы с выводом сообщений на экран
	order_function Inicialization;
	
	//Создаю сокет, который будет прослушивать нужный нам ip адрес и порт
	SOCKET listen_socket = A.create_socket();
	cout << "listener socket: " << listen_socket << endl;

	// структура SockAddr, содержащая данные, необходимые для подключения 
	sockaddr_in TSockAddr = Inicialization.set_struct_sockaddr_in(_port, _ip_aadr);
	
	//связывание структуры TSockAddr с созданным сокетом. 
	A.bind_struct_with_socket(listen_socket, reinterpret_cast<SOCKADDR*>(&TSockAddr), sizeof(TSockAddr));

	//перевожу сокет в слушающий режим
	if (SOCKET_ERROR == listen(listen_socket, 10)) cout << "ERROR WITH listen";
	
	cout << "Ip_address: " << inet_ntoa(TSockAddr.sin_addr) << " Server is listening to port " << _port << ", waiting for connections... id thread: " << this_thread::get_id() << endl;

	//Список свех подключившихся пользоватлей и файл, содержащий этих пользователей
	vector<char*> list_of_users;
	HANDLE vector_users_file = CreateFile(_T("users/list_users(vector)"), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	//Жду запросов)
	while (1)
	{
		//структура для подключившегося клиента!
		sockaddr_in TSockAddr_new;
		
		socket_wrap_client C;
		int sizet = sizeof(TSockAddr_new);
		
		//жду соединения
		SOCKET _socket = C.accept_message(listen_socket, (SOCKADDR*)&TSockAddr_new, &sizet);
		
		//класс, который запишет информацию о нашем пользователе в файл и выведет информацию о нем на экран
		Inicialization.set_information_about_new_user(vector_users_file, TSockAddr_new);

		//Записал нового пользователя в вектор
		list_of_users.push_back(inet_ntoa(TSockAddr_new.sin_addr));

		//Для каждого подключившегося пользователя создаем обработчик
		DWORD pidThread = 0;
		
		Params p;
		p.socket = _socket;
		p.TSockAddr = TSockAddr_new;
		HANDLE handleThraed = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)handler, (LPVOID)&p, 0, &pidThread);
		//WaitForSingleObject(handleThraed, INFINITE);
	}
	
	closesocket(listen_socket);
	return 1;
}


//http://www.uhlib.ru/kompyutery_i_internet/sistemnoe_programmirovanie_v_srede_windows/p12.php
//http://iantonov.me/page/proxy-server-na-delphi-i-winsock-api
//https://club.shelek.ru/viewart.php?id=35
TEST(parcer, minParsing) {
	size_t s = minParsing(-3, 4);
	ASSERT_EQ(4, s);
	s = minParsing(3, 4);
	ASSERT_EQ(3, s);
}
TEST(parcer, getMethod) {
	parcerRequest test_str("POST http://www.site.ru/news.html HTTP/1.0\r\nHost: www.site.ru\r\nReferer: http://www.site.ru/index.html\r\nCookie: income = 1\r\nContent-Type: multipart/form-data; boundary = 1BEF0A57BE110FD467A\r\nContent-Length: 209\r\n\r\n");
	test_str.getMethod();
	ASSERT_STREQ("POST", test_str.getInformation().Method.data());
}
TEST(parcer, getDate) {
	parcerRequest test_str("HTTP/1.1 200 OK\r\nDate: Mon, 27 Jul 2009 12 : 28 : 53 GMT\r\nServer: Apache / 2.2.14 (Win32)\r\nLast - Modified: Wed, 22 Jul 2009 19 : 15 : 56 GMT\r\nContent-Length: 88\r\nContent-Type: text / html\r\nConnection: Closed\r\n\r\n<html><body><h1>Hello, World!< / h1>< / body>< / html>\r\n");
	test_str.getDate();
	ASSERT_STREQ("Mon, 27 Jul 2009 12 : 28 : 53 GMT", test_str.getInformation().Date.data());
}
TEST(parcer, getRequestURL) {
	parcerRequest test_str("POST http://www.site.ru/news.html HTTP/1.0\r\nHost: www.site.ru\r\nReferer: http://www.site.ru/index.html\r\nCookie: income = 1\r\nContent-Type: multipart/form-data; boundary = 1BEF0A57BE110FD467A\r\nContent-Length: 209\r\n\r\n");
	test_str.getRequestURL();
	ASSERT_STREQ("http://www.site.ru/news.html", test_str.getInformation().RequestURL.data());
}
TEST(parcer, getContentType) {
	parcerRequest test_str("POST http://www.site.ru/news.html HTTP/1.0\r\nHost: www.site.ru\r\nReferer: http://www.site.ru/index.html\r\nCookie: income = 1\r\nContent-Type: multipart/form-data; boundary = 1BEF0A57BE110FD467A\r\nContent-Length: 209\r\n\r\n");
	test_str.getContentType();
	ASSERT_STREQ("multipart/form-data", test_str.getInformation().ContentType.data());
}
TEST(parcer, getContentLength) {
	parcerRequest test_str("POST http://www.site.ru/news.html HTTP/1.0\r\nHost: www.site.ru\r\nReferer: http://www.site.ru/index.html\r\nCookie: income = 1\r\nContent-Type: multipart/form-data; boundary = 1BEF0A57BE110FD467A\r\nContent-Length: 209\r\n\r\n");
	test_str.getContentLength();
	ASSERT_EQ(209, test_str.getInformation().ContentLength);
}
TEST(parcer, getHost) {
	parcerRequest test_str("POST http://www.site.ru/news.html HTTP/1.0\r\nHost: www.site.ru\r\nReferer: http://www.site.ru/index.html\r\nCookie: income = 1\r\nContent-Type: multipart/form-data; boundary = 1BEF0A57BE110FD467A\r\nContent-Length: 209\r\n\r\n");
	test_str.getHost();
	ASSERT_STREQ("www.site.ru", test_str.getInformation().Host.data());
}
int main(int argc, char** argv)
{
	
	
	::testing::InitGoogleTest(&argc, argv);
	//receiver(40, "192.168.79.1");

	receiver(40, "172.20.10.2");
	
	//receiver(8080, "192.168.43.59");
	return RUN_ALL_TESTS();
	//http://www.firststeps.ru/mfc/winapi/r.php?117
}


