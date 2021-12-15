/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseResponse.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:44:34 by sgah              #+#    #+#             */
/*   Updated: 2021/12/15 05:01:57 by sgah             ###   ########.fr       */
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
					std::cout << tmp << std::endl;
					return i->second;
				}
		}
		else
		{
			std::string suffix(i->first.substr(1));
			if (locationName.size() >= suffix.size() && !locationName.compare(locationName.size() - suffix.size(), suffix.size(), suffix))
				return i->second;
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
	size_t start;
	size_t end = 0;

	while ((start = content.find_first_of("/", end)) != std::string::npos &&
		(end = content.find_first_of("/", start + 1)) != std::string::npos)
		if (start == end - 1)
		content.erase(start, end - start);
	return (content);
}

void		Parser::parseResponse(ConfigResponse& confResponse, Request& request, Config& server)
{
	std::string	locationName(request.getPath());
	Config		location(findLocation(server, locationName));
	std::string	content;

	confResponse.setRequest(request);
	confResponse.setServer(server);
	confResponse.setLocationFile(request.getPath());
	confResponse.setLocationPath(locationName);
	confResponse.setLocation(location);
	confResponse.setErrorMap(location.getErrorPage());
	confResponse.setClientBodyBufferSize(location.getClientBodyBufferSize());
	confResponse.setCgiParam(location.getCgiParam());
	confResponse.setCgiPass(location.getCgiPass());
	confResponse.setAllowMethod(location.getAllowedMethods());
	confResponse.setLanguage(setLanguage(request.getHeader("Accept-Language")));
	confResponse.setAutoIndex(location.getAutoIndex());
	confResponse.setIndex(location.getIndex());

	std::cout << location << std::endl;
	if (!location.getAlias().empty() && *locationName.begin() != '*')
		content = location.getAlias() + checkContentLocation(request.getPath().substr(locationName.size()));
	else
		content = location.getRoot() + checkContentLocation(request.getPath());

	std::cout << "THIS IS "+content << std::endl;
	confResponse.setContentLocation(content);

}
