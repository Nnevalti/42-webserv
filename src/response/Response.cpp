/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 18:34:09 by sgah              #+#    #+#             */
/*   Updated: 2021/12/07 02:21:54 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(void) {}

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

void	Response::initDirectives(void)
{
	_directives["_allow"] = "";
	_directives["_contentLanguage"] = "";
	_directives["_contentLength"] = "";
	_directives["_contentLocation"] = "";
	_directives["_contentType"] = "";
	_directives["_date"] = "";
	_directives["_lastModified"] = "";
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

static int		checkPath(const std::string& path)
{
	struct stat s;
	if (stat(path.c_str(), &s) == 0 )
	{
		if (s.st_mode & S_IFDIR)
			return 2; // IS A DIRECTORY
		else if (s.st_mode & S_IFREG)
			return 1; //IS A REGULAR FILE
		else
			return 0; //SOMETHING ELSE
	}
	else
		return (-1); //ERROR
}

void	Response::initResponse(void)
{
	if (_code == 200)
		(this->*Response::_method[_request.getMethod()])();

}
void	Response::createBody(void)
{

}

void	Response::createHeader(void)
{

}

void	Response::getMethod(void)
{

}



