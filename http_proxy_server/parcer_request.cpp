#include "stdafx.h"
#include "parcer_request.hpp"
#include <iostream>
#include <string>
#define DateType std::string
using namespace std;

size_t minParsing(size_t a1, size_t a2) {
	if (a1 < 0) return a2;
	if (a2 < 0) return a1;
	if (a1 < a2) return a1;
	else return a2;
}
parcerRequest::parcerRequest(string url) {
	_url = url; dataRequest _data; 
}
void parcerRequest::getMethod() {
	size_t index = _url.find(" ");
	_data.Method = string(_url, 0, index);
}
bool parcerRequest::getDate() {
	size_t index = _url.find("Date") + 6;
	if (index < 0) return 0;
	_data.Date = string(_url, index);
	index = _data.Date.find("\r");
	size_t indexEnd = _data.Date.find(";");
	_data.Date = string(_data.Date, 0, minParsing(index, indexEnd));
	//cout << _data.Date;
	return 1;
}
bool parcerRequest::getRequestURL() {
	size_t index = _url.find(" ") + 1;
	if (index < 0) return 0;
	_data.RequestURL = string(_url, index);
	index = _data.RequestURL.find("\r");
	size_t indexEnd = _data.RequestURL.find(" ");
	_data.RequestURL = string(_data.RequestURL, 0, minParsing(index, indexEnd));
	//cout << _data.RequestURL;
	return 1;
}
bool parcerRequest::getContentLength() {
	size_t index = _url.find("Content-Length") + 18;
	if (index < 0) return 0;
	string s = string(_url, index);
	index = s.find("\r");
	size_t indexEnd = s.find(";");
	_data.ContentLength = atoi(string(s, 0, minParsing(index, indexEnd)).data());
	//cout << _data.ContentLength;
	return 1;
}
bool parcerRequest::getContentType() {
	size_t index = _url.find("Content-Type") + 16;
	if (index < 0) return 0;
	string s = string(_url, index);
	index = s.find("\r");
	size_t indexEnd = s.find(";");
	_data.ContentType = string(s, 0, minParsing(index, indexEnd)).data();
	//cout << _data.ContentType;
	return 1;
}
bool parcerRequest::getContentDisposition() {
	size_t index = _url.find("Content-Disposition") + 23;
	if (index < 0) return 0;
	string s = string(_url, index);
	index = s.find("\r");
	size_t indexEnd = s.find(";");
	_data.ContentDisposition = string(s, 0, minParsing(index, indexEnd)).data();
	//cout << _data.ContentDisposition;
	return 1;
}
bool parcerRequest::getReferer() {
	size_t index = _url.find("Referer") + 9;
	if (index < 0) return 0;
	_data.Referer = string(_url, index);
	index = _data.Referer.find("\r");
	size_t indexEnd = _data.Host.find(";");
	_data.Referer = string(_data.Referer, 0, minParsing(index, indexEnd));
	//cout << _data.Referer;
	return 1;
}
bool parcerRequest::getHost() {
	size_t index = _url.find("Host") + 6;
	if (index < 0) return 0;
	_data.Host = string(_url, index);
	index = _data.Host.find("\r");
	size_t indexEnd = _data.Host.find(";");
	_data.Host = string(_data.Host, 0, minParsing(index, indexEnd));
	//cout << _data.Host;
	return 1;
}
bool parcerRequest::getCookie() {
	size_t index = _url.find("Cookie") + 8;
	if (index < 0) return 0;
	_data.Cookie = string(_url, index);
	index = _data.Cookie.find("\r");
	size_t indexEnd = _data.Cookie.find(";");
	_data.Cookie = string(_data.Cookie, 0, minParsing(index, indexEnd));
	//cout << _data.Cookie;
	return 1;
}
bool parcerRequest::getHTTPVersion() {
	size_t index = _url.find("HTTP/");
	if (index < 0) return 0;
	_data.HTTPVersion = string(_url, index);
	index = _data.HTTPVersion.find("\r");
	size_t indexEnd = _data.HTTPVersion.find(";");
	_data.HTTPVersion = string(_data.HTTPVersion, 0, minParsing(index, indexEnd));
	//cout << _data.HTTPVersion;
	return 1;
}
bool parcerRequest::getData() {
	size_t index = _url.find("\r\n\r\n");
	if (index < 0) return 0;

}
void parcerRequest::parceAll() {
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
}