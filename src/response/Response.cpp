/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 18:34:09 by sgah              #+#    #+#             */
/*   Updated: 2021/12/10 15:26:05 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(void) {}

Response::Response(Response const & src)
{
	*this = src;
}

Response::~Response(void) {}

Response &	Response::operator=(const Response &rhs)
{
	_config = rhs._config;
	return (*this);
}

std::string	Response::getResponse(void) const
{
	return (_response);
}

methodMap	Response::initMethods(void)
{
	methodMap map;

	map["GET"] = &Response::getMethod;

	return map;
}

methodMap Response::_method = Response::initMethods();

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
	return (tmp);
}

stringMap Response::_typeMap = Response::initType();

std::string	Response::findType(std::string contentlocation)
{
	std::string type(contentlocation.substr(contentlocation.rfind(".") + 1, contentlocation.size() - contentlocation.rfind(".")));
	if (_typeMap.find(type) == _typeMap.end())
		return "text/plain";
	return _typeMap[type];
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
	_directives["Location"] = "";
	_directives["Retry-After"] = "";
	_directives["Server"] = "Webserv 0.1";
	_directives["Transfer-Encoding"] = "identity";
	_directives["WwwAuthenticate"] = "";
}

void			Response::initErrorMap(void)
{
	_errors[100] = "Continue";
	_errors[200] = "OK";
	_errors[201] = "Created";
	_errors[204] = "No Content";
	_errors[400] = "Bad Request";
	_errors[403] = "Forbidden";
	_errors[404] = "Not Found";
	_errors[405] = "Method Not Allowed";
	_errors[413] = "Payload Too Large";
	_errors[500] = "Internal Server Error";
}

void		Response::resetResponse(ConfigResponse& conf)
{
	_code = 200;
	_header = "";
	initErrorMap();
	initDirectives();
	_config = conf;
}

static std::string	getDate(void)
{
	char			buffer[100];
	struct timeval	tv;
	struct tm		*tm;

	gettimeofday(&tv, NULL);
	tm = gmtime(&tv.tv_sec);
	strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
	return (std::string(buffer));
}

static std::string	getLastModif(const std::string& path)
{
	char			buffer[100];
	struct stat		stats;
	struct tm		*tm;

	if (stat(path.c_str(), &stats) == 0)
	{
		tm = gmtime(&stats.st_mtime);
		strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
		return (std::string(buffer));
	}
	return ("");
}

void		Response::createHeader(int code)
{
	stringVector tmp(_config.getAllow());

	_header += "HTTP/1.1 " + (static_cast<std::ostringstream*>( &(std::ostringstream() << code) )->str());
	_header += " " + _errors[code] + "\r\n";
	for (stringVector::iterator i = tmp.begin(); i != tmp.end(); i++)
		if (i + 1 == tmp.end())
			_directives["Allow"] += *i;
		else
			_directives["Allow"] += *i + " ";
	_directives["Content-Language"] = _config.getLanguage();
	//IF the error dont have a file
	_directives["Content-Length"] = "0";
	if (code != 404)
		_directives["Content-Location"] = _config.getContentLocation();
	_directives["Date"] = getDate();
	_directives["Last-Modified"] = getLastModif(_config.getContentLocation());
	_directives["Content-Type"] = "";
	if (code == 503 || code == 429 || code == 301)
		_directives["Retry-After"] = "2";
	if (code == 401)
		_directives["WWW-Authenticate"] = "Basic realm=\"Access requires authentification\" charset=\"UTF-8\"";
	for (stringMap::const_iterator i = _directives.begin(); i != _directives.end(); i++)
		if (i->second != "")
			_header+= i->first + ": " + i->second + "\r\n";
	_response = _header + "\r\n";
}

void		Response::InitResponseProcess(void)
{
	stringSet tmp(_config.getAllowMethod());

	if (tmp.find(_config.getRequest().getMethod()) == tmp.end())
		_code = 405;
	else if (_config.getClientBodyBufferSize() < _config.getRequest().getBody().size())
		_code = 413;
	if (_code != 200)
		createHeader(_code);
	else
		(this->*Response::_method[_config.getRequest().getMethod()])();
}

void		Response::getMethod(void)
{

}
