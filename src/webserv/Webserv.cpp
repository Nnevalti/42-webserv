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

static bool g_run = true;

void signal_handler(int signum)
{
	(void)signum;
	g_run = false;
}

Webserv::Webserv(void) {}

Webserv::~Webserv(void) {}

void Webserv::initServers(confVector configServer)
{
	// int			fd;

	_servers = configServer;
	for (confVector::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		netVector net = (*it).getNetwork();

		for (netVector::const_iterator it_net = net.begin(); it_net != net.end(); it_net++)
		{
			// _listeningPorts.insert(it_net->port);
			std::cout << "server name: " << (*it).getServerName().front() << '\n';
			std::cout << "port: " << it_net->port << "\n\n";
		}
	}
	// for (setPort::iterator it = _listeningPorts.begin(); it != _listeningPorts.end(); it++)
	// {
	// 	fd = init_socket(*it);
	// 	_servers_fd.push_back(fd);
	// }
	std::cout << "All servers were built" << std::endl;
	return ;
}

int Webserv::init_socket(int port)
{
	int listen_fd;
	const int opt = 1;
	struct sockaddr_in servaddr;

	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "error: socket() failed" << std::endl;
		exit(1);
	}

	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, &opt, sizeof(opt)))
	{
		std::cerr << "error: setsockopt() failed" << std::endl;
		exit(1);
	}

	if(fcntl(listen_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "could not set socket to be non blocking" << std::endl;
		exit(1);
	}

	std::memset((char*)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(port);

	if (bind(listen_fd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
	{
		std::cerr << "error: bind() failed" << std::endl;
		exit(1);
	}

	if (listen(listen_fd, 100) < 0)
	{
		std::cerr << "error: listen() failed" << std::endl;
		exit(1);
	}
	return listen_fd;
}

void Webserv::epoll_init(void)
{
	_epfd = epoll_create1(0);
	for (fd_vector::iterator it = _servers_fd.begin(); it != _servers_fd.end(); it++)
	{
		_event.data.fd = *it;
		_event.events = EPOLLIN;
		epoll_ctl(_epfd, EPOLL_CTL_ADD, *it, &_event);
	}
}

int	Webserv::fd_is_server(int ready_fd)
{
	for (fd_vector::iterator it = _servers_fd.begin(); it != _servers_fd.end(); it++)
		if (*it == ready_fd)
			return *it;
	return 0;
}

void Webserv::run(confVector configServer)
{
	initServers(configServer);
	return ;
	/************************/
	/*    Temporary part    */
	/************************/
	char request[1024];
	std::string response;
	response += "HTTP/1.1 200 OK\n";
	response += "Content-Type: text/html\r\n";
	response += "Content-Length: 14\n\n";
	response += "Hello World !\n\r\n\r\n";

	std::string  wait[] = {"⠋", "⠙", "⠸", "⠴", "⠦", "⠇"}; // array of frame
	int n = 0; // current frame
	/*************************/
	/* End of temporary part */
	/*************************/

	int timeout = 200; // set timeout to 0.5 sec
	int nfds = 0;
	int new_socket = 0;

	signal(SIGINT, signal_handler); // to move in main
	epoll_init();
	while (g_run)
	{
		errno = 0;

		// Verify if a new connection is available
		nfds = epoll_wait(_epfd, _events_pool, MAX_EV, timeout);
		/*
		errno value returned by epoll
		EBADF : epfd is not a valid file descriptor.

		EFAULT : The memory area pointed to by events is not accessible with write permissions.

		EINTR : The call was interrupted by a signal handler before either any of the requested events occurred or the timeout expired.

		EINVAL : epfd is not an epoll file descriptor, or maxevents is less than or equal to zero.

		*/
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
				close(_events_pool[j].data.fd);
				continue;
			}
			// If the server has a new connection ready
			if ((server = fd_is_server(_events_pool[j].data.fd)) > 0)
			{
				if ((new_socket = accept(server, NULL, NULL)) < 0)
				{
					if(errno != EWOULDBLOCK)
					{
						std::cerr << "error: accept() failed" << std::endl;
						exit(1);
					}
				}
				std::cout << "\r" << "Client connected on server: " << _events_pool[j].data.fd << std::endl;
				fcntl(new_socket, F_SETFL, O_NONBLOCK);
				_event.data.fd = new_socket;
				_event.events = EPOLLIN;
				epoll_ctl(_epfd, EPOLL_CTL_ADD, new_socket, &_event);
			}
			else if (_events_pool[j].events & EPOLLIN)
			{
				int ret = 0;

				// Receive the request
				if ((ret = recv(_events_pool[j].data.fd, &request, 1023, 0)) < 0)
				{
					std::cerr << "error: recv() failed" << strerror(errno) << std::endl;
					exit(1);
				}
				else
				{
					request[ret] = '\0';
					std::cout << request << std::endl;
					_event.events = EPOLLOUT;
					_event.data.fd = _events_pool[j].data.fd;
					epoll_ctl(_epfd, EPOLL_CTL_MOD, _events_pool[j].data.fd, &_event);
				}
				break ;
			}
			else if (_events_pool[j].events & EPOLLOUT)
			{
				if(send(_events_pool[j].data.fd, response.c_str(), response.size(), 0) < 0)
				{
					std::cerr << "error: send() failed" << std::endl;
					exit(1);
				}
				_event.events = EPOLLIN;
				_event.data.fd = _events_pool[j].data.fd;
				// if not keep-alive
				close(_events_pool[j].data.fd);
				epoll_ctl(_epfd, EPOLL_CTL_MOD, _events_pool[j].data.fd, &_event);
				break ;
			}
		}
	}
	for (fd_vector::iterator it = _servers_fd.begin(); it != _servers_fd.end(); it++)
		close(*it);
	close(_epfd);
	std::cout << "\r" << "Serveur ending...       " << std::endl;
	exit(1);
}
