/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 18:34:09 by sgah              #+#    #+#             */
/*   Updated: 2021/12/07 20:42:57 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(void): _response(""){}

Response::Response(Response const & src)
{
	*this = src;
}

Response::~Response(void) {}

Response&	Response::operator=(const Response& rhs)
{
	_response = rhs._response;
	_code = rhs._code;
	_server = rhs._server;
	_request = rhs._request;
	_errorMap = rhs._errorMap;
	_directives = rhs._directives;

	return (*this);
}

methodMap	Response::initMethods()
{
	methodMap map;

	map["GET"] = &Response::getMethod;

	return map;
}

methodMap Response::_method = Response::initMethods();

stringMap Response::_typeMap = Response::initType();

stringMap	Response::initType()
{
	stringMap tmp;

	tmp["html"] = "text/html";
	tmp["css"] = "text/css";
	tmp["js"] = "text/javascript";
	tmp["jpeg"] = "image/jpeg";
	tmp["jpg"] = "image/jpeg";
	tmp["png"] = "image/png";
	tmp["bmp"] = "image/bmp";
	tmp[""] = "text/plain";
}

void	Response::initDirectives(void)
{
	_directives["Allow"] = "";
	_directives["Content-Language"] = "";
	_directives["Content-Length"] = "";
	_directives["Content-Location"] = "";
	_directives["Content-Type"] = "";
	_directives["Date"] = "";
	_directives["Last-Modified"] = "";
	_directives["_location"] = "";
	_directives["_retryAfter"] = "";
	_directives["_server"] = "";
	_directives["_transferEncoding"] = "";
	_directives["_wwwAuthenticate"] = "";
}

void	Response::setCode(int code)
{
	_code = code;
}

void	Response::setErrorMap(StringIntVectorMap page)
{
	for (StringIntVectorMap::const_iterator i = page.begin(); i != page.end(); i++)
	{
		std::vector<int> tmp(i->second);

		for ( std::vector<int>::iterator it = tmp.begin(); it != tmp.end(); i++)
			_errorMap[*it] = i->first;
	}
}

void	Response::setRequest(Request& request)
{
	_request = request;
}

void	Response::setServer(Config& server)
{
	_server = server;
}

void	Response::setIndex(stringVector index)
{
	_index = index;
}

void	Response::setContentLocation(std::string location)
{
	_contentLocation = location;
}

static int		checkPath(const std::string& path)
{
	struct stat s;
	if (stat(path.c_str(), &s) == 0 )
	{
		if (s.st_mode & S_IFDIR)
			return IS_A_DIRECTORY; // IS A DIRECTORY
		else if (s.st_mode & S_IFREG)
			return IS_A_FILE; //IS A REGULAR FILE
		else
			return IS_SOMETHING_ELSE; //SOMETHING ELSE
	}
	else
		return (-1); //ERROR
}

void	Response::initResponse(void)
{
	if (_code == 200)
		(this->*Response::_method[_request.getMethod()])();

}

void	Response::readFile(void)
{

}

void	Response::createBody(void)
{

}

void	Response::createHeader(void)
{
	if (_code == 405)
	{
		stringVector tmp(_server.getAllowedMethods());

		for (stringVector::const_iterator i = tmp.begin(); i != tmp.end(); i++)
			_directives["Allow"] += ((i + 1 == tmp.end()) ? *i : *i + ", ");
	}

	_directives["Content-Language"] = _request.getHeader("Accept-Language");

	_directives["Content-Length"] = _response.size();

	if (_code != 404)
		_directives["Content-Location"] = _contentLocation;

	std::string type(_contentLocation.substr(_contentLocation.rfind(".") + 1, _contentLocation.size() - _contentLocation.rfind(".")));
	_directives["Content-Type"] = this->_typeMap[type];

	char			buffer[100];
	struct timeval	tv;
	struct tm		*tm;
	struct stat		stats;

	gettimeofday(&tv, NULL);
	tm = gmtime(&tv.tv_sec);
	strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
	_directives["Date"] = std::string(buffer);

	std::memset(buffer, 0, 100);
	if (stat(_contentLocation.c_str(), &stats) == 0)
	{
		tm = gmtime(&stats.st_mtime);
		strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
		_directives["Last-Modified"] = std::string(buffer);
	}


}

void	Response::getMethod(void)
{
	std::string		path(_request.getPath());

	if (checkPath(path) == IS_A_FILE)
	{
		std::ifstream		file(path.c_str());
		std::stringstream	buff;

		if (file.is_open())
			buff << file.rdbuf();
		else
		{
			_code = 403;
			return ;
		}
		_response = buff.str();
	}
	createHeader();
}
