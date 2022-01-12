/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 20:06:10 by sgah              #+#    #+#             */
/*   Updated: 2022/01/11 19:15:06 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

stringVector		Parser::initMethods()
{
	stringVector	methods;

	methods.push_back("GET");
	methods.push_back("POST");
	methods.push_back("DELETE");

	return methods;
}

stringVector	Parser::methods = Parser::initMethods();

void			Parser::splitHeader(Request &request)
{
	std::string tmp;
	size_t		start(0);
	size_t		end;

	while((end = request.raw_request.find_first_of('\n', start)) != std::string::npos)
	{
		tmp = request.raw_request.substr(start, end - start);
		if (tmp == "")
			break;
		if (tmp[tmp.size() - 1] == '\r' && tmp.size())
			tmp.resize(tmp.size() - 1);
		request.header.push_back(tmp);
		start = (end == std::string::npos) ? end : end + 1;
	}
	return ;
}

void			Parser::parseFirstLine(Request &request)
{
	std::string	first(request.header.front());
	size_t		end;
	size_t		start;

	if ((end = first.find_first_of(' ')) == std::string::npos)
	{
		request.setRet(400);
		std::cerr << RED << "\rError parsing request: missing first line element" << SET << std::endl;
		return ;
	}
	request.setMethod(first.substr(0, end));

	if (std::find(methods.begin(), methods.end(), request.getMethod()) == methods.end())
	{
		request.setRet(400);
		std::cerr << RED << "\rError parsing request: wrong method" << SET << std::endl;
		return ;
	}

	if ((start = first.find_first_not_of(' ', end))  == std::string::npos)
	{
		request.setRet(400);
		std::cerr << RED << "\rError parsing request: missing first line element" << SET << std::endl;
		return ;
	}

	if ((end = first.find_first_of(' ', start)) == std::string::npos)
	{
		request.setRet(400);
		std::cerr << RED << "\rError parsing request: missing first line element" << SET << std::endl;
		return ;
	}
	request.setPath(first.substr(start, end - start));

	if ((end = first.find_first_not_of(' ', end)) == std::string::npos)
	{
		request.setRet(400);
		std::cerr << RED << "\rError parsing: missing first line element" << SET << std::endl;
		return ;
	}

	std::string version;

	if (first.substr(end, 5).compare("HTTP/") == 0)
		request.setVersion((version = first.substr(end + 5, 3)));
	else
		return;

	if (version != "1.0" && version != "1.1")
	{
		request.setRet(400);
		std::cerr << RED << "Error parsing request: HTTP/" << version << " find in request header" << SET << std::endl;
		return ;
	}
}

static std::string	findtoken(std::string line)
{
	return (line.substr(0, line.find_first_of(':')));
}

static std::string	findvalue(std::string line)
{
	return (line.substr(line.find_first_of(' ') + 1));
}

void			Parser::parseHeader(Request &request)
{
	std::string	token;
	std::string	value;

	// can we move this in resetRequest ?
	request.resetDirective();

	splitHeader(request);
	parseFirstLine(request);

	for (stringVector::iterator i = request.header.begin() + 1; i != request.header.end(); i++)
	{
		token = findtoken(*i);
		value = findvalue(*i);
		if (request.getHeaders().count(token))
		{
			request.setHeader(token, value);
			request.setEnvForCgi(token, value);
		}
	}
	request.setNetwork(request.getHeader("Host"));

}

void			Parser::parseBody(Request& request)
{
	std::string		body;
	size_t			body_start(0);

	body_start = request.raw_request.find("\r\n\r\n");
	if ((body_start = request.raw_request.find("\r\n\r\n")) == std::string::npos)
		std::cout << "ERROR in parse Body" << '\n';
	if (request.getCode() == 400)
		return ;

	body = request.raw_request.substr(body_start + 4); // +4 to skip the "\r\n\r\n"
	// std::cout << "**************REQUEST CONTENT" << '\n';
	// std::cout << request.raw_request << '\n';
	// std::cout << "**************REQUEST CONTENT END" << '\n';
	// std::cout << "**************Content Length Header: " << request.getHeader("Content-Length").c_str() << '\n';
	// std::cout << "**************Body Size: " << (int)body.size() << '\n';
	request.bodySize = request.contentSize - body_start - 4;
	if ((unsigned long)std::atol(request.getHeader("Content-Length").c_str()) != request.contentSize - body_start - 4)
	{
		request.setRet(400);
		std::cerr << RED << "Error parsing request: content length different from content real size" << SET << std::endl;
		return ;
	}
	else
		request.setBody(body);
}
