/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 20:06:10 by sgah              #+#    #+#             */
/*   Updated: 2021/12/01 16:04:35 by sgah             ###   ########.fr       */
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

void			Parser::parseHeader(const std::string& request, stringVector& header)
{
	std::string tmp;
	size_t		start(0);
	size_t		end;

	std::cout << "pass in here\n" << request;

	while((end = request.find_first_of('\n', start)) != std::string::npos)
	{

		tmp = request.substr(start, end - start);
		if (tmp == "\r")
			break;
		header.push_back(tmp);
		start = (end == std::string::npos) ? end : end + 1;
	}
}

void			Parser::parseRequest(const std::string& request, Request classRequest)
{
	stringVector	header;
	Request			ret;

	classRequest.resetDirective();
	parseHeader(request, header);

	std::cout << "request: " << header.size() << std::endl;
	for (stringVector::iterator i = header.begin(); i != header.end(); i++)
	{
		std::cout << *i << std::endl;
	}

}
