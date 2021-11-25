/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdescham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 13:49:50 by vdescham          #+#    #+#             */
/*   Updated: 2021/11/25 13:49:50 by vdescham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void) {}

Server::Server(Server const & src)
{
	*this = src;
}

Server&		Server::operator=(const Server &rhs)
{
	if (this != &rhs)
		_config = rhs.getConfig();
	return *this;
}

Server::~Server(void) {}

void Server::setConfig(Config config)
{
	this->_config = config;
	// std::cout << "IN SETCONFIG" << '\n';
	// std::cout << _config << std::endl;
}

Config Server::getConfig(void) const
{
	return this->_config;
}
