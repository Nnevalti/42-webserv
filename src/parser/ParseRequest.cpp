/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 20:06:10 by sgah              #+#    #+#             */
/*   Updated: 2021/12/15 18:38:32 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

stringVector		Parser::initMethods()
{
	stringVector	methods;

	methods.push_back("GET");
	methods.push_back("HEAD");
	methods.push_back("POST");
	methods.push_back("PUT");
	methods.push_back("DELETE");

	return methods;
}

stringVector	Parser::methods = Parser::initMethods();

size_t			Parser::splitHeader(const std::string& request, stringVector& header)
{
	std::string tmp;
	size_t		start(0);
	size_t		end;

	while((end = request.find_first_of('\n', start)) != std::string::npos)
	{
		tmp = request.substr(start, end - start);
		if (tmp == "")
			break;
		if (tmp[tmp.size() - 1] == '\r' && tmp.size())
			tmp.resize(tmp.size() - 1);
		header.push_back(tmp);
		start = (end == std::string::npos) ? end : end + 1;
	}
	return (start);
}

void			Parser::parseFirstLine(stringVector& header, Request& classRequest)
{
	std::string	first(header.front());
	size_t		end;
	size_t		start;

	if ((end = first.find_first_of(' ')) == std::string::npos)
	{
		classRequest.setRet(400);
		std::cerr << RED << "Error parsing request: missing first line element" << SET << std::endl;
		return ;
	}
	classRequest.setMethod(first.substr(0, end));

	if (std::find(methods.begin(), methods.end(), classRequest.getMethod()) == methods.end())
	{
		classRequest.setRet(400);
		std::cerr << RED << "Error parsing request: wrong method" << SET << std::endl;
		return ;
	}

	if ((start = first.find_first_not_of(' ', end))  == std::string::npos)
	{
		classRequest.setRet(400);
		std::cerr << RED << "Error parsing request: missing first line element" << SET << std::endl;
		return ;
	}

	if ((end = first.find_first_of(' ', start)) == std::string::npos)
	{
		classRequest.setRet(400);
		std::cerr << RED << "Error parsing request: missing first line element" << SET << std::endl;
		return ;
	}
	classRequest.setPath(first.substr(start, end - start));

	if ((end = first.find_first_not_of(' ', end)) == std::string::npos)
	{
		classRequest.setRet(400);
		std::cerr << RED << "Error parsing: missing first line element" << SET << std::endl;
		return ;
	}

	std::string version;

	if (first.substr(end, 5).compare("HTTP/") == 0)
		classRequest.setVersion((version = first.substr(end + 5, 3)));
	else
		return;

	if (version != "1.0" && version != "1.1")
	{
		classRequest.setRet(400);
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

void			Parser::parseHeader(stringVector& header, Request& classRequest)
{
	parseFirstLine(header, classRequest);
	std::string	token;
	std::string	value;

	if (classRequest.getCode() == 400)
		return ;

	for (stringVector::iterator i = header.begin() + 1; i != header.end(); i++)
	{
		token = findtoken(*i);
		value = findvalue(*i);
		if (classRequest.getHeaders().count(token))
		{
			classRequest.setHeader(token, value);
			classRequest.setEnvForCgi(token, value);
		}
	}
}

void			Parser::parseRequest(const std::string& request, Request& classRequest)
{
	stringVector	header;
	std::string		body("");
	Request			ret;
	size_t			body_start(0);

	classRequest.resetDirective();
	body_start = splitHeader(request, header);

	parseHeader(header, classRequest);

	if (classRequest.getCode() == 400)
		return ;

	if(classRequest.getMethod() == "POST")
	{
		body = request.substr(body_start);
		if (std::atoi(classRequest.getHeader("Content-Length").c_str()) != (int)body.size())
		{
			classRequest.setRet(400);
			std::cerr << RED << "Error parsing request: content length different from content real size" << SET << std::endl;
			return ;
		}
		else
			classRequest.setBody(body);
	}
	classRequest.setNetwork(classRequest.getHeader("Host"));
}
