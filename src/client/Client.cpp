/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 13:49:50 by vdescham          #+#    #+#             */
/*   Updated: 2021/12/06 20:35:50 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void)
{
	// handle timeout
	gettimeofday(&last_request, NULL);
}

Client::Client(Client const & src)
{
	*this = src;
}

Client&		Client::operator=(const Client &rhs)
{
	if (this != &rhs)
	{
		request = rhs.request;
		last_request = rhs.last_request;
		_socket = rhs._socket;
		_request = rhs._request;
		_server = rhs._server;
	}
	return *this;
}

Client::~Client(void) {}

void Client::setSocket(int socket) { _socket = socket; }
int Client::getSocket(void) { return _socket; }
// std::string Client::getRequest(void) { return _request; }

void Client::setServer(Config server)
{
	_server = server;
}

Config& Client::getServer(void)
{
	return _server;
}
