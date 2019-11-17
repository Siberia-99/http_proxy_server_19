#pragma once
#include "stdafx.h"
#include <string>
#include <regex>
#include <iostream>

#define DateType std::string

struct dataRequest { //Request-Line [ General-Header | Request-Header | Entity-Header ]\r\n[ Entity-Body ]
					 //RequestLine 
	std::string RequestLineStr{ "" };//"Method Request-URI HTTP-Version\r\n" 
	std::string Method; //GET, POST
	std::string RequestURL;///index.html или http://www.myhost.ru/index.html или /index.html?a=1&b=qq
	std::string HTTPVersion;//HTTP/1.0

					   //GenerealHeader 
	std::string GeneralHeaderStr{ "" };// [Date: value\n | Pragma: no-cache\n] 
	std::string Host;
	DateType Date; //Date  дата по Гринвичу в формате "День недели, Число Месяц Год ЧЧ:ММ:СС GMT"
	bool isCache; //pragma:no-cache?

				  //RequestHeader 
	std::string RequestHeaderStr{ "" };
	std::string Allow;//"Allow: GET | HEAD\n"
	std::string From;//"From: myname@mailserver.ru\r\n". 
	DateType ifModifiedSince; //dlya Get Дата указывается по Гринвичу в таком же формате, как и для параметра Date в General-Header. 
	std::string Referer; //"Referrer: www.host.ru/index.html\n". 
	std::string UserAgent; //"User-Agent: Mozilla/4.0\n" 

					  //EntityHeader 
	std::string EntityHeaderStr{ "" };
	//string Allow;//"Allow: GET | BODY\n"
	std::string ContentEncoding;
	int ContentLength;//"Content-Length: 26457\n".
	std::string ContentType;//"Content-Type: text/html\n"
	std::string ContentDisposition;
	DateType Expires;//"Expires: date\n". Формат даты алогичен формату даты для параметра Date  - когда должна быть удалена из кэша
	DateType LastModified; //время последнего изменения пересылаемых данных
	std::string ExtentionHeader; //"ParameterName: parametervalue\n". Данные параметры будут игнорироваться, если программа-клиент не знает, как их обработать.Например: "Cookie: r=1\r\n"  устанавливает всем известные печеньки для страницы.
	std::string Cookie;
};
size_t minParsing(size_t a1, size_t a2);

class parcerRequest //GET http://www.php.net/ HTTP/1.0\r\n\r\n 
{
public:
	parcerRequest(std::string url);
	//~parcerRequest();
	void getMethod();
	bool getDate();
	bool getRequestURL();
	bool getContentType();
	bool getContentDisposition();
	bool getContentLength();
	bool getReferer();
	bool getHost();
	bool getCookie();
	bool getHTTPVersion();
	void parceAll();
	bool getData();
	dataRequest getInformation() const;
private:
	dataRequest _data;
	std::string _url;
};