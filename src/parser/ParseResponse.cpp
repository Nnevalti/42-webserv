/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseResponse.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:44:34 by sgah              #+#    #+#             */
/*   Updated: 2021/12/14 20:47:51 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Config		Parser::findLocation(Config& server, std::string& locationName)
{
	serverMap locations(server.getLocation());

	std::cout << "Location name is : " <<locationName << std::endl;
	for (serverMap::iterator i = locations.begin(); i != locations.end(); i++)
		std::cout << i->first << ":" << i->second;

	if (locationName.empty())
		return server;
	if (locations.empty())
		return server;

	for (serverMap::const_iterator i = locations.begin(); i != locations.end(); i++)
		for (std::string tmp = locationName; !tmp.empty(); tmp.resize(tmp.size() - 1))
		{
			std::cout << "tmp state is :" << tmp << std::endl;
			if (tmp == i->first)
				return i->second;
		}
	return server;
}

void		Parser::parseResponse(ConfigResponse& confResponse, Request& request, Config& server)
{
	std::string	locationName(request.getPath());
	Config		location(findLocation(server, locationName));

	confResponse.setRequest(request);
	confResponse.setServer(server);
	confResponse.setLocationFile(request.getPath());
	confResponse.setLocationPath(locationName);
	confResponse.setLocation(location);
	confResponse.setErrorMap(server.getErrorPage());
	confResponse.setClientBodyBufferSize(server.getClientBodyBufferSize());
	confResponse.setCgiParam(location.getCgiParam());
	confResponse.setCgiPass(location.getCgiPass());
	confResponse.setAllowMethod(server.getAllowedMethods());
	confResponse.setLanguage(request.getHeader("Accept-Language"));
	confResponse.setAutoIndex(server.getAutoIndex());
	confResponse.setIndex(location.getIndex());

	std::cout << location << std::endl;
	std::cout << (location.getAlias().empty() ? "NO" : "YES") << std::endl;
	if (location.getAlias().empty())
		confResponse.setContentLocation(location.getRoot() + location.getAlias() + request.getPath());
	else
		confResponse.setContentLocation(location.getRoot() + request.getPath());
}
