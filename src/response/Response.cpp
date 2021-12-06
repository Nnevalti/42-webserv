/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 18:34:09 by sgah              #+#    #+#             */
/*   Updated: 2021/12/06 21:14:52 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(void) {}

Response::Response(Response const & src)
{
	*this = src;
}

Response::~Response(void) {}

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




