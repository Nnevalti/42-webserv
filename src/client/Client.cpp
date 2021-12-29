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

Client::Client(void) {}

Client::Client(Client const & src)
{
	*this = src;
}

Client&		Client::operator=(const Client &rhs)
{
	if (this != &rhs)
	{

	}
	return *this;
}

Client::~Client(void) {}

void Client::setSocket(int socket) { _socket = socket; }
int Client::getSocket(void) { return _socket; }
Client::listReq &Client::getRequests(void) { return _requests; }
// std::string Client::getRequest(void) { return _request; }

void Client::addRequest(Request &request)
{
	_requests.push_back(request);
}

void Client::removeRequest(listReq::iterator pos)
{
	_requests.erase(pos);
}

void Client::setServer(Config server)
{
	_server = server;
}

Config& Client::getServer(void)
{
	return _server;
}
