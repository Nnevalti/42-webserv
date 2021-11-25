/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 19:53:36 by sgah              #+#    #+#             */
/*   Updated: 2021/11/22 20:26:43 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include <typeinfo>
Webserv::Webserv(void) {}

Webserv::Webserv(Webserv const & src)
{
	(void)src;
}

Webserv::~Webserv(void) {}

void Webserv::buildServers(confVector configServer)
{
	std::cout << "\nIn build servers :" << '\n';

	Server server;
	for (confVector::iterator it = configServer.begin(); it != configServer.end(); it++)
	{
		server.setConfig(*it);
		// _listeningPorts.insert(*it.getNetwork());
		_servers.push_back(server);
		std::cout << _servers.front().getConfig() << '\n';
	}
	for (serverVector::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		netVector net = (*it).getConfig().getNetwork();

		for (netVector::const_iterator it_net = net.begin(); it_net != net.end(); it_net++)
		{
			std::cout << "host: " << inet_ntoa(it_net->host) << '\n';
			std::cout << "port: " << it_net->port << "\n\n";
		}

	}
	std::cout << "All servers were built" << std::endl;
	return ;
}
