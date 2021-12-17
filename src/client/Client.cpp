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

Client::Client(void): _header_ready(false) {}

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

// int Client::readRequest(void)
// {
// 	char client_request[BUFFER_SIZE + 1] = {0};
// 	int ret = recv(_socket, &client_request, BUFFER_SIZE, 0);
//
// 	if (ret == -1)
// 	{
// 		std::cerr << "Error: recv failed, closing connection" << '\n';
// 		return -1;
// 	}
// 	else if (ret == 0)
// 	{
// 		std::cout << "Closing connection request from clients" << '\n';
// 		return -1;
// 	}
// 	else
// 	{
// 		_request += client_request;
// 		if (_header_ready == false && _request.find(:\r\n\r\n) == std::string::npos)
// 			return 2;
// 		else
// 		{
// 			// get Method if it is post and the body is not ready return 2 else return 1
// 			// Here parse Header partially and compare content-length to the body
// 			// also chunked verification wil be done here
// 		}
// 		// std::cout << _request << std::endl;
// 		return 2;
// 	}
// 	return ;
//
// }

void Client::sendResponse()
{
}
