/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 13:49:50 by vdescham          #+#    #+#             */
/*   Updated: 2022/01/18 03:24:59 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void) : hadResponse(false), userId("")
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
		hadResponse = rhs.hadResponse;
		_socket = rhs._socket;
		_server = rhs._server;
		userId = rhs.userId;
		cookies = rhs.cookies;
	}
	return *this;
}

Client::~Client(void) {}

void Client::setSocket(int socket) { _socket = socket; }
int Client::getSocket(void) { return _socket; }

void Client::setServer(Config server)
{
	_server = server;
}

Config& Client::getServer(void)
{
	return _server;
}
