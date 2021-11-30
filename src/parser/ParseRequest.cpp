/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 20:06:10 by sgah              #+#    #+#             */
/*   Updated: 2021/11/30 21:41:30 by sgah             ###   ########.fr       */
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

stringVector	Parser::getAllLines(std::string request)
{
	stringVector ret;
	size_t		start;
	size_t		end(0);

	request += "\n";

	while ((start = request.find_first_not_of("\n", end)) != std::string::npos &&
		(end = request.find_first_of("\n", start)) != std::string::npos)
	{
		ret.push_back(request.substr(start, end - start));
	}
	return ret;
}

void			Parser::parseRequest(const std::string& request)
{
//	stringVector	header(getAllLines(request));
	Request			ret;

	(void)request;
	_request = ret;
}

Request			Parser::getRequest(void) const
{
	return _request;
}

