#include "stdafx.h"
#include "Parser.hpp"

Parser::Parser()
{
}

Parser::~Parser()
{
}

void Parser::parse_request(string _request)
{
	size_t i = _request.find("://");
	if (i != string::npos)
	{
		type = 1;
		protocol = _request.substr(0, i);
		_request.erase(0, i + 3);
	}
	else
	{
		type = 2;
		protocol = "http";
	}

	i = _request.find("/");
	if (i == string::npos)
	{
		host = _request;
		path = "";
	}
	else
	{
		host = _request.substr(0, i);
		path = _request.substr(i);
	}
}

