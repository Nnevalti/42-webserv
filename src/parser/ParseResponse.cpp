/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseResponse.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:44:34 by sgah              #+#    #+#             */
/*   Updated: 2022/01/18 03:34:25 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Config		Parser::findLocation(Config& server, std::string& locationName)
{
	serverMap locations(server.getLocation());

	if (*locationName.end() == '/')
		locationName.resize(locationName.size() - 1);

	if (locationName.empty())
		return server;
	if (locations.empty())
		return server;

	for (serverMap::const_iterator i = locations.begin(); i != locations.end(); i++)
		if (i->first[0] != '*')
		{
			for (std::string tmp = locationName; !tmp.empty(); tmp.resize(tmp.size() - 1))
				if (tmp == i->first)
				{
					locationName = tmp;
					return i->second;
				}
		}
		else
		{
			std::string suffix(i->first.substr(1));
			if (locationName.size() > suffix.size() && !locationName.compare(locationName.size() - suffix.size(), suffix.size(), suffix))
			{
				Config ret(i->second);

				ret.extension = true;
				return ret;
			}
		}

	return server;
}

std::string	Parser::setLanguage(std::string acceptLanguage)
{
	if (acceptLanguage == "*")
		return ("en");
	else if (acceptLanguage.size() <= 3)
		return (acceptLanguage);
	else
		return (acceptLanguage.substr(0, acceptLanguage.find_first_of('-')));
}

std::string				Parser::checkContentLocation(std::string content)
{
	const char	*tmp = content.c_str();
	std::string	ret;
	int			i = 0;
	bool		was = false;

	while(tmp[i])
	{
		if (tmp[i] == '/')
		{
			if (was == false && tmp[i + 1])
				ret.push_back(tmp[i]);
			was = true;
		}
		else
		{
			ret.push_back(tmp[i]);
			was = false;
		}
		i++;
	}
	return (ret);
}

void		Parser::parseCookies(ConfigResponse& confResponse, Request& request)
{
	std::map<std::string, std::string>	cookiesToSet;
	std::string							charSet(" ;");
	std::string							cookies(request.getHeader("Cookie"));

	cookies += " ";

	size_t start = cookies.find_first_not_of(charSet, 0);
	size_t end = 0;

	while ((end = cookies.find_first_of(charSet, start)) != std::string::npos)
	{
		std::string	tmp = cookies.substr(start, end - start);

		std::string token;
		std::string value;

		token.assign(tmp, 0, tmp.find_first_of('='));
		value.assign(tmp, tmp.find_first_of('=') + 1, tmp.size());

		cookiesToSet[token] = value;

		if ((start = cookies.find_first_not_of(charSet, end)) == std::string::npos)
			break ;
	}
	confResponse.setCookies(cookiesToSet);
}

void		Parser::parseResponse(ConfigResponse& confResponse, Request& request, Config& server)
{
	std::string	locationName(request.getPath());
	Config		location(findLocation(server, locationName));
	std::string	content;

	parseCookies(confResponse, request);
	confResponse.setCode(request.getCode());
	confResponse.setRequest(request);
	confResponse.setServer(server);
	confResponse.setLocationFile(request.getPath());
	confResponse.setLocationPath(locationName);
	confResponse.setLocation(location);
	confResponse.setErrorMap(location.getErrorPage());
	confResponse.setClientBodyBufferSize(location.getClientBodyBufferSize());
	confResponse.setCgiPass(location.getCgiPass());
	confResponse.setAllowMethod(location.getAllowedMethods());
	confResponse.setLanguage(setLanguage(request.getHeader("Accept-Language")));
	confResponse.setAutoIndex(location.getAutoIndex());
	confResponse.setIndex(location.getIndex());

	if (!location.getAlias().empty() && location.extension == false)
		content = location.getRoot() + location.getAlias() + request.getPath().substr(locationName.size());
	else if (!location.getAlias().empty() && location.extension)
		content = location.getRoot() + location.getAlias() + locationName;
	else
		content = location.getRoot() + request.getPath();

	content = checkContentLocation(content);

	confResponse.setContentLocation(content);

}
