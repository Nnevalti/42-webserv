/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 19:53:36 by sgah              #+#    #+#             */
/*   Updated: 2021/12/15 18:15:08 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include <typeinfo>

volatile bool g_run = true;

Webserv::Webserv(void) {}

Webserv::~Webserv(void) {}

/*
	add to webserv the parser class directly
*/
void	Webserv::setParser(Parser& parser)
{
	_parser = parser;
}

void Webserv::init()
{
	_servers = _parser.getConfigServers();

	epollCreate();
	initServers();
	epollInit();
}

void Webserv::epollCreate(void)
{
	_epfd = epoll_create1(0);
	if (_epfd == -1)
		throw std::runtime_error("Error: Could not initialised epoll");
}

/*
	Takes the config vector and create the servers sockets that will listen for connection.
	We avoid IP/Port pair duplication
*/
void Webserv::initServers(void)
{
	for (confVector::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		t_network net = it->getNetwork();
		_servers_fd.push_back(init_socket(net));
	}
}

/*
	Used in initServers syscall to create a socket with the correct IP/Port
	Set the socket to be reusable and non-blocking
*/
int Webserv::init_socket(t_network network)
{
	int listen_fd;
	const int opt = 1;
	struct sockaddr_in servaddr;

	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw std::logic_error("error: socket() failed");
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::logic_error("error: setsockopt() failed");
	if (fcntl(listen_fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::logic_error("error: fcntl() failed");

	std::memset((char*)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = network.host.s_addr;
	servaddr.sin_port = htons(network.port);

	if (servaddr.sin_addr.s_addr == static_cast<in_addr_t>(-1))
		throw std::logic_error("error: inet_addr: Invalid IP");
	if (bind(listen_fd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
		throw std::logic_error("error: bind() failed");
	if (listen(listen_fd, MAX_CLIENTS) < 0)
		throw std::logic_error("error: listen() failed");
	return listen_fd;
}

/*
	Add all the listening socket decriptor in the epoll list
*/
void Webserv::epollInit(void)
{
	std::memset((struct epoll_event *)&_event, 0, sizeof(_event));
	for (fdVector::iterator it = _servers_fd.begin(); it != _servers_fd.end(); it++)
	{
		_event.data.fd = *it;
		_event.events = EPOLLIN;
		epoll_ctl(_epfd, EPOLL_CTL_ADD, *it, &_event);
	}
}

/*
	Verify if the redy fd pass in parameters is a server listening socket and not a client socket
*/
bool	Webserv::fd_is_server(int ready_fd)
{
	for (fdVector::iterator it = _servers_fd.begin(); it != _servers_fd.end(); it++)
		if (*it == ready_fd)
			return true;
	return false;
}

/*
	Accept a client connection on one of the listening socket and create a new_socket for the connection
*/
void	Webserv::accept_new_client(int server)
{
	Client client;
	int new_socket = 0;

	if ((new_socket = accept(server, NULL, NULL)) < 0)
	{
		if(errno != EWOULDBLOCK)
			throw std::logic_error("Error: accept() failed");
	}
	std::cout << "\r" << "Client connected on server: " << server << std::endl;
	if(fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0)
		throw std::logic_error("Error: fcntl() failed");

	_event.data.fd = new_socket;
	_event.events = EPOLLIN;
	epoll_ctl(_epfd, EPOLL_CTL_ADD, new_socket, &_event);

	client.setSocket(new_socket);
	_clients[new_socket] = client;
}


void	Webserv::getRightServer(Client &client)
{
	bool foundAConf = false;
	Config rightConf;

	for (confVector::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		if (client.request.getNetwork() == it->getNetwork())
		{
			rightConf = *it;
			foundAConf = true;
			std::cout << "CONFIG:\nIP/PORT:" << it->getNetwork() << '\n';
			break ;
		}
	}
	if (!foundAConf)
		std::cout << "No corresponding server" << '\n';
	client.setServer(rightConf);
}

void	Webserv::read_client_request(int clientSocket)
{
	char client_request[BUFFER_SIZE + 1];
	int ret = 0;

	if ((ret = recv(clientSocket, &client_request, BUFFER_SIZE, 0)) < 0)
		throw std::logic_error("error: recv() failed");
	else if (ret == 0)
	{
		std::cout << "Closing connection request from clients" << '\n';
		epoll_ctl(_epfd, EPOLL_CTL_DEL, clientSocket, NULL);
		close(clientSocket);
		_clients.erase(clientSocket);
	}
	else
	{
		client_request[ret] = '\0';
		if (_clients[clientSocket].request.raw_request == "")
			_clients[clientSocket].request.raw_request = client_request;
		else
			_clients[clientSocket].request.raw_request += client_request;
		// std::cout << client_request << std::endl;
		if (_clients[clientSocket].request.raw_request.find("\r\n\r\n") != std::string::npos
			&& _clients[clientSocket].request.header_ready == false)
		{
			std::cout << "**************HEADER READY" << '\n';
			_clients[clientSocket].request.header_ready = true;
			_parser.parseHeader(_clients[clientSocket].request);

			if (_clients[clientSocket].request.getMethod() != "POST")
			{
				_clients[clientSocket].request.body_ready = true;
				return ;
			}

		}
		else if (_clients[clientSocket].request.header_ready == true)
		{
			std::string body("");
			body = _clients[clientSocket].request.raw_request.substr(_clients[clientSocket].request.raw_request.find("\r\n\r\n") + 4);

			if ((int)body.size() >= std::atoi(_clients[clientSocket].request.getHeader("Content-Length").c_str()))
			{
				std::cout << "**************Content Length Header: " << _clients[clientSocket].request.getHeader("Content-Length").c_str() << '\n';
				std::cout << "**************Body Size: " << (int)body.size() << '\n';
				_parser.parseBody(_clients[clientSocket].request);
				_clients[clientSocket].request.body_ready = true;
			}
		}
	}
	return ;
}

/*
	Receive a client request and stores it in a string
*/
void Webserv::handleRead(int client_fd)
{
	read_client_request(client_fd);
	 if (_clients[client_fd].request.body_ready == true)
	{
		// std::cout << "BODY READY" << '\n';
		// std::cout << _clients[client_fd].request << '\n';

		// parse the body and set client to EPOLLOUT
		_event.events = EPOLLOUT;
		_event.data.fd = client_fd;
		epoll_ctl(_epfd, EPOLL_CTL_MOD, client_fd, &_event);
	}

}

void Webserv::handleWrite(int client_fd)
{
	Response	classResponse;
	ConfigResponse confResponse;
	std::string	response;

	// forward request to the right server
	getRightServer(_clients[client_fd]);
	// ***************************************************
	// Try to make this four lines in a response::function
	std::cout << "*******************CLIENT REQUEST" << '\n';
	std::cout << _clients[client_fd].request << '\n';
	_parser.parseResponse(confResponse, _clients[client_fd].request, _clients[client_fd].getServer());
	classResponse.resetResponse(confResponse);
	classResponse.InitResponseProcess();
	response = classResponse.getResponse();
	// ***************************************************
	std::cout << "*******************RESPONSE" << '\n';
	std::cout << response << std::endl;
	// Change the line below
	_clients[client_fd].getRequests().clear();
	// Send response
	if(send(client_fd, response.c_str(), response.size(), 0) < 0)
		throw std::logic_error("error: send() failed");
	// listen client again for other requests and wait for a close connection request
	_event.events = EPOLLIN;
	_event.data.fd = client_fd;
	epoll_ctl(_epfd, EPOLL_CTL_MOD, client_fd, &_event);
	std::cout << "******************* REQUEST ENDED HERE" << '\n';

	// clear clients request once response is done
	_clients[client_fd].request.header_ready = false;
	_clients[client_fd].request.body_ready = false;
	_clients[client_fd].request.raw_request = "";
	_clients[client_fd].request.header.clear();
	// Create the function below and usr it to reset the class
	// _clients[client_fd].request.resetRequest();
}

void Webserv::handleError(int socket)
{
	epoll_ctl(_epfd, EPOLL_CTL_DEL, socket, NULL);
	close(socket);
}
/*
	Main function of the Webserv class, the server loop is here
*/
void Webserv::run()
{
	int n = 0;
	std::string  wait[] = {"⠋", "⠙", "⠸", "⠴", "⠦", "⠇"};
	// replace the variable below by a macro
	int timeout = 200;
	int nfds = 0;
	std::string	request;

	init();
	while (g_run)
	{
		errno = 0;

		// Verify if a new connection is available
		nfds = epoll_wait(_epfd, _events_pool, MAX_EV, timeout);
		if (errno == EINVAL || errno == EFAULT || errno == EBADFD)
			std::cerr << "error: epoll_wait() failed: " << strerror(errno) << '\n';
		else if (errno == EINTR)
			g_run = false;
		for (int j = 0; j < nfds; j++)
		{
			if (_events_pool[j].events & EPOLLERR || _events_pool[j].events & EPOLLHUP)
				handleError(_events_pool[j].data.fd);
			else if (_events_pool[j].events & EPOLLIN && fd_is_server(_events_pool[j].data.fd))
				accept_new_client(_events_pool[j].data.fd);
			else if (_events_pool[j].events & EPOLLIN)
				handleRead(_events_pool[j].data.fd);
			else if (_events_pool[j].events & EPOLLOUT)
				handleWrite(_events_pool[j].data.fd);
		}
		if (nfds == 0)
		{
			std::cout << "\r" << wait[(n++ % 6)] << GREEN << " waiting for connection" << SET << std::flush;
			// garbage_collector();
		}

	}
	for (fdVector::iterator it = _servers_fd.begin(); it != _servers_fd.end(); it++)
		close(*it);
	close(_epfd);
	std::cout << "\r" << "Serveur ending...       " << std::endl;
}
