/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 18:34:09 by sgah              #+#    #+#             */
/*   Updated: 2021/12/14 20:55:54 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

static int		checkPath(const std::string& path)
{
	struct stat s;

	if (stat(path.c_str(), &s) == 0 )
	{
		if (s.st_mode & S_IFDIR)
			return 2;
		else if (s.st_mode & S_IFREG)
			return 1;
		else
			return 0;
	}
	else
		return 0;
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
	_body = "";
	initErrorMap();
	initDirectives();
	_config = conf;
}

std::string			Response::readFile(int code)
{
	std::string	path(_config.getErrorPath(code));
	std::ofstream		file;
	std::stringstream	buffer;

	if (checkPath(path) == 1)
	{
		file.open(path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
		{
			_code = 404;
			readFile(_config.getErrorMap()[404]);
			return ((static_cast<std::ostringstream*>( &(std::ostringstream() << _body.size()) )->str()));
		}
		buffer << file.rdbuf();
		file.close();
		_directives["Content-Type"] = "text/html";
		_body = buffer.str();
		return ((static_cast<std::ostringstream*>( &(std::ostringstream() << _body.size()) )->str()));
	}
	else
	{
		_code = 404;
		readFile(_config.getErrorMap()[404]);
	}
	return ((static_cast<std::ostringstream*>( &(std::ostringstream() << _body.size()) )->str()));
}

std::string			Response::readFile(std::string path)
{
	std::ofstream		file;
	std::stringstream	buffer;

	if (checkPath(path) == 1)
	{
		file.open(path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
		{
			_code = 404;
			_body = "<!DOCTYPE html>\n<html>\n\t<title>404 Not Found</title>\n\t<body>\n\t\t<div>\n\t\t\t<H1>404 Not Found</H1>\n\t\t</div>\n\t</body>\t</html>";
			return ((static_cast<std::ostringstream*>( &(std::ostringstream() << _body.size()) )->str()));
		}
		buffer << file.rdbuf();
		file.close();
		_directives["Content-Type"] = "text/html";
		_body = buffer.str();
		return ((static_cast<std::ostringstream*>( &(std::ostringstream() << _body.size()) )->str()));
	}
	else
	{
		_code = 404;
		_body = "<!DOCTYPE html>\n<html>\n\t<title>404 Not Found</title>\n\t<body>\n\t\t<div>\n\t\t\t<H1>404 Not Found</H1>\n\t\t</div>\n\t</body>\t</html>";
	}
	return ((static_cast<std::ostringstream*>( &(std::ostringstream() << _body.size()) )->str()));
}

void		Response::createHeader(void)
{
	stringVector tmp(_config.getAllow());

	if(_code == 405)
	{
		for (stringVector::iterator i = tmp.begin(); i != tmp.end(); i++)
			if (i + 1 == tmp.end())
				_directives["Allow"] += *i;
			else
				_directives["Allow"] += *i + " ";
	}
	_directives["Content-Language"] = _config.getLanguage();
	if (_code != 200)
		_directives["Content-Length"] = readFile(_code);
	else
		_directives["Content-Length"] = readFile(_config.getContentLocation());
	if (_code != 404)
		_directives["Content-Location"] = _config.getContentLocation();
	_directives["Date"] = getDate();
	_directives["Last-Modified"] = getLastModif(_config.getContentLocation());
	if (_code == 503 || _code == 429 || _code == 301)
		_directives["Retry-After"] = "2";
	if (_code == 401)
		_directives["WWW-Authenticate"] = "Basic realm=\"Access requires authentification\" charset=\"UTF-8\"";

	_header += "HTTP/1.1 " + (static_cast<std::ostringstream*>( &(std::ostringstream() << _code) )->str());
	_header += " " + _errors[_code] + "\r\n";
	for (stringMap::const_iterator i = _directives.begin(); i != _directives.end(); i++)
		if (i->second != "")
			_header+= i->first + ": " + i->second + "\r\n";
	_response = _header + "\r\n" + _body;
}

void		Response::InitResponseProcess(void)
{
	stringSet tmp(_config.getAllowMethod());

	if (tmp.find(_config.getRequest().getMethod()) == tmp.end())
		_code = 405;
	else if (_config.getClientBodyBufferSize() < _config.getRequest().getBody().size())
		_code = 413;

	if (_code != 200)
		createHeader();
	else if(_method.find(_config.getRequest().getMethod()) != _method.end())
		(this->*Response::_method[_config.getRequest().getMethod()])();
}

void		Response::getMethod(void)
{
	std::string tmp(_config.getContentLocation());

	//todo FIX NOT COMPLETE FIND THE RIGHT WAY TO GET INDEX
	std::cout << tmp << std::endl;
	if (tmp[tmp.size() - 1] == '/')
		_config.setContentLocation(tmp + _config.getIndex().front());
	createHeader();
}
