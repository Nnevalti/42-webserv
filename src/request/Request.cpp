/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 19:51:17 by sgah              #+#    #+#             */
/*   Updated: 2021/11/30 21:24:13 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(void): _method (""), _version(""), _ret(200), _body(""), _port(8080), _path(""), _query("") {}

Request::Request(Request const & src)
{
	*this = src;
}

Request::~Request(void) {}

const std::list<std::pair<std::string, float> >&	Request::getLang() const
{
	return _lang;
}

const std::map<std::string, std::string>&	Request::getEnv() const
{
	return _env_for_cgi;
}

Request &			Request::operator=(const Request &rhs)
{
	(void)rhs;
	return (*this);
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

const std::string&			Request::getQuery() const
{
	return _query;
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

int				Request::getStatus(void) const
{
	return (_status);
}
