#include "stdafx.h"
#include "parcer_request.hpp"
#include <iostream>
#include <string>
#define DateType std::string
using namespace std;

int minParsing(int a1, int a2) {
	if (a1 < 0) return a2;
	if (a2 < 0) return a1;
	if (a1 < a2) return a1;
	else return a2;
}
ParserRequest::ParserRequest(string url) {
	_url = url; dataRequest _data; 
}
void ParserRequest::getMethod() {
	int index = _url.find(" ");
	_data.Method = string(_url, 0, index);
}
bool ParserRequest::getDate() {
	int index = _url.find("Date") + 6;
	if (index < 0) return 0;
	_data.Date = string(_url, index);
	index = _data.Date.find("\r");
	int indexEnd = _data.Date.find(";");
	_data.Date = string(_data.Date, 0, minParsing(index, indexEnd));
	//cout << _data.Date;
	return 1;
}
bool ParserRequest::getRequestURL() {
	int index = _url.find(" ") + 1;
	if (index < 0) return 0;
	_data.RequestURL = string(_url, index);
	index = _data.RequestURL.find("\r");
	int indexEnd = _data.RequestURL.find(" ");
	_data.RequestURL = string(_data.RequestURL, 0, minParsing(index, indexEnd));
	//cout << _data.RequestURL;
	return 1;
}
bool ParserRequest::getContentLength() {
	int index = _url.find("Content - Length") + 18;
	if (index < 0) return 0;
	string s = string(_url, index);
	index = s.find("\r");
	int indexEnd = s.find(";");
	_data.ContentLength = atoi(string(s, 0, minParsing(index, indexEnd)).data());
	//cout << _data.ContentLength;
	return 1;
}
bool ParserRequest::getReferer() {
	int index = _url.find("Referer") + 9;
	if (index < 0) return 0;
	_data.Referer = string(_url, index);
	index = _data.Referer.find("\r");
	int indexEnd = _data.Host.find(";");
	_data.Referer = string(_data.Referer, 0, minParsing(index, indexEnd));
	//cout << _data.Referer;
	return 1;
}
bool ParserRequest::getHost() {
	int index = _url.find("Host") + 6;
	if (index < 0) return 0;
	_data.Host = string(_url, index);
	index = _data.Host.find("\r");
	int indexEnd = _data.Host.find(";");
	_data.Host = string(_data.Host, 0, minParsing(index, indexEnd));
	//cout << _data.Host;
	return 1;
}
bool ParserRequest::getCookie() {
	int index = _url.find("Cookie") + 8;
	if (index < 0) return 0;
	_data.Cookie = string(_url, index);
	index = _data.Cookie.find("\r");
	int indexEnd = _data.Cookie.find(";");
	_data.Cookie = string(_data.Cookie, 0, minParsing(index, indexEnd));
	//cout << _data.Cookie;
	return 1;
}
bool ParserRequest::getHTTPVersion() {
	int index = _url.find("HTTP/");
	if (index < 0) return 0;
	_data.HTTPVersion = string(_url, index);
	index = _data.HTTPVersion.find("\r");
	int indexEnd = _data.HTTPVersion.find(";");
	_data.HTTPVersion = string(_data.HTTPVersion, 0, minParsing(index, indexEnd));
	//cout << _data.HTTPVersion;
	return 1;
}