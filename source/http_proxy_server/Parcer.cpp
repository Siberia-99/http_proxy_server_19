#include "stdafx.h"
#include "Parcer.hpp"

parcer::parcer()
{
}

parcer::~parcer()
{
}

void parcer::parse_request(string _request)
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

