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

/*
	Takes the config vector and create the servers sockets that will listen for connection.
	We avoid IP/Port pair duplication
*/
void Webserv::initServers(confVector configServer)
{
	_servers = configServer;

	for (confVector::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		t_network net = it->getNetwork();

		_servers_fd.push_back(init_socket(net));
	}
	return ;
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
	Init the epoll fd and add all the listening socket decriptor in the epoll list
*/
void Webserv::epoll_init(void)
{
	_epfd = epoll_create1(0);
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
int	Webserv::fd_is_server(int ready_fd)
{
	for (fdVector::iterator it = _servers_fd.begin(); it != _servers_fd.end(); it++)
		if (*it == ready_fd)
			return *it;
	return 0;
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
			throw std::logic_error("error: accept() failed");
	}
	std::cout << "\r" << "Client connected on server: " << server << std::endl;
	if(fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0)
		throw std::logic_error("error: fcntl() failed");
	_event.data.fd = new_socket;
	_event.events = EPOLLIN;
	epoll_ctl(_epfd, EPOLL_CTL_ADD, new_socket, &_event);
	client.setSocket(new_socket);
	_clients[new_socket] = client;
}

/*
	Receive a client request and stores it in a string
*/
void	Webserv::read_client_request(int clientSocket, std::string &request)
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
		request = "";
	}
	else
	{
		client_request[ret] = '\0';
		// std::cout << client_request << std::endl;
		request = client_request;
		_event.events = EPOLLOUT;
		_event.data.fd = clientSocket;
		epoll_ctl(_epfd, EPOLL_CTL_MOD, clientSocket, &_event);
	}
	return ;
}

bool	comparePath(std::string path, std::string root)
{
	std::string::iterator r_it = root.begin();
	std::string::iterator r_it2;
	std::string::iterator p_it;

	for (; r_it != root.end(); r_it++)
	{
		r_it2 = r_it;
		for (p_it = path.begin(); p_it != path.end(); p_it++, r_it2++)
		{
			if (*p_it != *r_it2)
				break ;
			if (r_it2 + 1 == root.end() || p_it + 1 == path.end())
				return true;
		}
	}
	return false;
}

bool compareMethod(std::string RequestMethod, stringVector allowedMethod)
{
	for (stringVector::iterator it = allowedMethod.begin(); it != allowedMethod.end(); it++)
	{
		if (RequestMethod == *it)
			return true;
	}
	return false;
}

void	Webserv::getRightServer(Client &client)
{
	bool foundAConf = false;
	Config rightConf;

	// std::cout << "/*************************************************/" << '\n';
	// std::cout << "/*                   getRightServer              */" << '\n';
	// std::cout << "/*************************************************/" << '\n';
	// if (!(client.getRequests().empty()))
	// 	std::cout << "REQUEST:\nHOST/PORT:" << client.getRequests().front().getNetwork() << '\n';
	// std::cout << '\n';

	for (confVector::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		if (client.getRequests().front().getNetwork() == it->getNetwork())
		{
			rightConf = *it;
			foundAConf = true;
			std::cout << "CONFIG:\nIP/PORT:" << it->getNetwork() << '\n';
			break ;
		}
	}
	if (!foundAConf)
		std::cout << "No corresponding server" << '\n';
	// std::cout << "/*************************************************/" << '\n';
	// std::cout << "/*                          END                  */" << '\n';
	// std::cout << "/*************************************************/" << '\n';
	// std::cout << rightConf << '\n';
	client.setServer(rightConf);
}

/*
	Main function of the Webserv class, the server loop is here
*/
void Webserv::run()
{
	int n = 0;
	std::string  wait[] = {"⠋", "⠙", "⠸", "⠴", "⠦", "⠇"};
	int timeout = 200;
	int nfds = 0;
	Response	classResponse;
	std::string	request;

	initServers(_parser.getConfigServers());
	epoll_init();
	while (g_run)
	{
		errno = 0;

		// Verify if a new connection is available
		nfds = epoll_wait(_epfd, _events_pool, MAX_EV, timeout);
		if (errno == EINVAL || errno == EFAULT || errno == EBADFD)
			std::cerr << "error: epoll_wait() failed: " << strerror(errno) << '\n';
		else if (errno == EINTR)
			g_run = false;
		if (nfds == 0)
			std::cout << "\r" << wait[(n++ % 6)] << GREEN << " waiting for connection" << SET << std::flush;

		for (int j = 0; j < nfds; j++)
		{
			int server = 0;
			if (_events_pool[j].events & EPOLLERR || _events_pool[j].events & EPOLLHUP)
			{
				// an error occured, we close the connection
				epoll_ctl(_epfd, EPOLL_CTL_DEL, _events_pool[j].data.fd, NULL);
				close(_events_pool[j].data.fd);
				continue ;
			}
			// If the server has a new connection ready
			if (_events_pool[j].events & EPOLLIN && (server = fd_is_server(_events_pool[j].data.fd)) > 0)
				accept_new_client(server);
			else if (_events_pool[j].events & EPOLLIN) // EPOLLIN : read
			{
				read_client_request(_events_pool[j].data.fd, request);
				if (!request.empty())
				{

					Request		classRequest;

					_parser.parseRequest(request, classRequest);
					_clients[_events_pool[j].data.fd].addRequest(classRequest);
					// std::cout << classRequest << std::endl;
				}
			}
			else if (_events_pool[j].events & EPOLLOUT) // EPOLLOUT : write
			{
				ConfigResponse confResponse;
				std::string	response;

				// forward request to the right server
				getRightServer(_clients[_events_pool[j].data.fd]);
				_parser.parseResponse(confResponse, _clients[_events_pool[j].data.fd].getRequests().front(), _clients[_events_pool[j].data.fd].getServer());
				classResponse.resetResponse(confResponse);
				classResponse.InitResponseProcess();
				response = classResponse.getResponse();
				// std::cout << response << std::endl;
				_clients[_events_pool[j].data.fd].getRequests().clear();
				// listen client again for other requests and wait for a close connection request
				if(send(_events_pool[j].data.fd, response.c_str(), response.size(), 0) < 0)
					throw std::logic_error("error: send() failed");
				_event.events = EPOLLIN;
				_event.data.fd = _events_pool[j].data.fd;
				epoll_ctl(_epfd, EPOLL_CTL_MOD, _events_pool[j].data.fd, &_event);
			}
		}
	}
	for (fdVector::iterator it = _servers_fd.begin(); it != _servers_fd.end(); it++)
		close(*it);
	close(_epfd);
	std::cout << "\r" << "Serveur ending...       " << std::endl;
}
