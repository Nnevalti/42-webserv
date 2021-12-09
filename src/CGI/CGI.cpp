/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdescham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 12:36:53 by vdescham          #+#    #+#             */
/*   Updated: 2021/12/07 12:36:53 by vdescham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI(void) {}

CGI::CGI(CGI const & src)
{
	*this = src;
}

CGI::~CGI(void) {}

CGI &	operator=(const CGI &rhs)
{

}

void CGI::initEnv(Request &request, Config &config)
{
	std::map<std::string, std::string>	headers = request.getHeaders();
	if (headers.find("Auth-Scheme") != headers.end() && headers["Auth-Scheme"] != "")
		this->_env["AUTH_TYPE"] = headers["Authorization"];

	this->_env["REDIRECT_STATUS"] = "200";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["SCRIPT_NAME"] = config.getCgiPass();
	this->_env["REQUEST_METHOD"] = request.getMethod();
	this->_env["CONTENT_LENGTH"] = std::itoa(this->_body.length());
	this->_env["CONTENT_TYPE"] = headers["Content-Type"];
	this->_env["PATH_INFO"] = request.getPath();
	this->_env["PATH_TRANSLATED"] = request.getPath();
	this->_env["QUERY_STRING"] = request.getQuery();
	this->_env["REMOTEaddr"] = inet_ntoa(config.getNetwork().host);

	this->_env["REMOTE_IDENT"] = headers["Authorization"];
	this->_env["REMOTE_USER"] = headers["Authorization"];

	// this->_env["REQUEST_URI"] = request.getPath() + request.getQuery();

	// if (headers.find("Hostname") != headers.end())
	// 	this->_env["SERVER_NAME"] = headers["Hostname"];
	// else
		this->_env["SERVER_NAME"] = this->_env["REMOTEaddr"];

	this->_env["SERVER_PORT"] = std::itoa(config.getNetwork().port);

	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["SERVER_SOFTWARE"] = "Webserv/1.0";

	this->_env.insert(config.getEnv().begin(), config.getEnv().end()); // env parameters from client request
}
