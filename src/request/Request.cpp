/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 19:51:17 by sgah              #+#    #+#             */
/*   Updated: 2021/12/02 02:49:23 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(void): _method (""), _version(""), _ret(200), _body(""), _port(8080), _path("") {}

Request::Request(Request const & src)
{
	*this = src;
}

Request::~Request(void) {}

Request&	Request::operator=(const Request &rhs)
{
	if (this != &rhs)
	{
		_method = rhs._method;
		_version = rhs._version;
		_ret = rhs._ret;
		_body = rhs._body;
		_port = rhs._port;
		_path = rhs._path;
		_headers = rhs._headers;
		_env_for_cgi = rhs._env_for_cgi;
	}
	return (*this);
}

void				Request::resetDirective(void)
{
	_version = "";
	_method = "";
	_ret = 200;
	_body = "";
	_port = 8080;
	_path = "";
	_headers.clear();
	_headers["Accept-Charsets"] = "";
	_headers["Accept-Language"] = "";
	_headers["Allow"] = "";
	_headers["Auth-Scheme"] = "";
	_headers["Authorization"] = "";
	_headers["Content-Language"] = "";
	_headers["Content-Length"] = "";
	_headers["Content-Location"] = "";
	_headers["Content-Type"] = "";
	_headers["Date"] = "";
	_headers["Host"] = "";
	_headers["Last-Modified"] = "";
	_headers["Location"] = "";
	_headers["Referer"] = "";
	_headers["Retry-After"] = "";
	_headers["Server"] = "";
	_headers["Transfer-Encoding"] = "";
	_headers["User-Agent"] = "";
	_headers["Cookie"] = "";
	_headers["Www-Authenticate"] = "";
	_headers["Connection"] = "Keep-Alive";
}

void										Request::setRet(int ret)
{
	_ret = ret;
}

void										Request::setMethod(std::string method)
{
	_method = method;
}

void										Request::setPath(std::string path)
{
	_path = path;
}

void										Request::setVersion(std::string version)
{
	_version = version;
}

void										Request::setHeader(std::string token, std::string value)
{
	_headers[token] = value;
}

void										Request::setNetwork(std::string IpPort)
{
	std::string	host;
	size_t		colons = IpPort.find(":");

	if ((host = IpPort.substr(0, colons)) == "localhost")
		host = "127.0.0.1";
	_network.host.s_addr = inet_addr(host.c_str());
	_network.port = std::atoi(IpPort.substr(++colons).c_str());
}


void										Request::setBody(std::string body)
{
	_body = body;
}


void										Request::setHeaders(stringMap headers)
{
	_headers = headers;
}

t_network									Request::getNetwork(void) const
{
	return (_network);
}

int											Request::getPort(void) const
{
	return _port;
}

const std::map<std::string, std::string>&	Request::getEnv() const
{
	return _env_for_cgi;
}

std::string		Request::getMethod(void) const
{
	return (_method);
}

std::string			Request::getPath() const
{
	return _path;
}

std::string		Request::getBody(void) const
{
	return _body;
}

int					Request::getRet() const
{
	return _ret;
}

std::string		Request::getVersion(void) const
{
	return (_version);
}

std::map<std::string, std::string>	Request::getHeaders(void) const
{
	return (_headers);
}

std::string		Request::getHeader(std::string str)
{
	return (_headers[str]);
}
