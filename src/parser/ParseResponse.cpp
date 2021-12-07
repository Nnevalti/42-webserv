/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseResponse.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:44:34 by sgah              #+#    #+#             */
/*   Updated: 2021/12/07 19:58:33 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Config				Parser::getLocationServer(Config& server, std::string path, std::string &name)
{
	serverMap	location(server.getLocation());


	if (path.empty())
		return (server);
	for (serverMap::iterator i = location.begin(); i != location.end(); i++)
	{
		std::string	tmp(path);
		size_t		size = tmp.size();

		while (tmp.size())
		{
			if (i->first == tmp)
			{
				name = tmp;
				return (i->second);
			}
			tmp.resize(--size);
		}
	}
	return (server);
}

void					Parser::parseResponse(Request& request, Response& response, Config& server)
{
	Config		location;
	std::string	locationName("");
	std::string	name;

	response.initDirectives();
	response.setErrorMap(server.getErrorPage());
	response.setServer(server);
	response.setRequest(request);
	response.setIndex(server.getIndex());

	location = getLocationServer(server, request.getPath(), locationName);

	if (location.getisAliasSet())
		name = server.getRoot() + location.getAlias() + request.getPath().substr(locationName.size());
	else
		name = server.getRoot() + request.getPath();
	std::cout << "HEEEEEERRRRRRREEEEEE: " + name << std::endl;
	response.setContentLocation(name);
	if (std::find(server.getAllowedMethods().begin(), server.getAllowedMethods().end(), request.getMethod()) == server.getAllowedMethods().end())
		response.setCode(405);
	else if (server.getClientBodyBufferSize() < request.getBody().size())
		response.setCode(413);
	response.setCode(200);
}
