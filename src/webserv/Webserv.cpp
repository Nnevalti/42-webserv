/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 19:53:36 by sgah              #+#    #+#             */
/*   Updated: 2022/01/17 18:41:48 by sgah             ###   ########.fr       */
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

bool Webserv::is_already_launch(confVector::iterator it_v)
{
	for (confVector::iterator it = _servers.begin(); it != it_v; it++)
		if (it_v->getNetwork() == it->getNetwork())
			return true;
	return false;
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
		if (!is_already_launch(it))
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
	{
		closeServersSocket();
		throw std::logic_error("Error: socket() failed");
	}
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, &opt, sizeof(opt)))
	{
		closeServersSocket();
		throw std::logic_error("Error: setsockopt() failed");
	}
	if (fcntl(listen_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		closeServersSocket();
		throw std::logic_error("Error: fcntl() failed");
	}

	std::memset((char*)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = network.host.s_addr;
	servaddr.sin_port = htons(network.port);

	if (servaddr.sin_addr.s_addr == static_cast<in_addr_t>(-1))
	{
		closeServersSocket();
		throw std::logic_error("Error: inet_addr: Invalid IP");
	}
	if (bind(listen_fd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
	{
		closeServersSocket();
		throw std::logic_error("Error: bind() failed");
	}
	if (listen(listen_fd, MAX_CLIENTS) < 0)
	{
		closeServersSocket();
		throw std::logic_error("Error: listen() failed");
	}
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
		{
			std::cerr << "Error: accept() failed" << '\n';
			return ;

		}
	}
	// std::cout << "\rClient connected on server: " << server << " " << new_socket << std::endl;
	if(fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "Error: fcntl() failed" << '\n';
		return ;
	}

	_event.data.fd = new_socket;
	_event.events = EPOLLIN;
	epoll_ctl(_epfd, EPOLL_CTL_ADD, new_socket, &_event);

	client.setSocket(new_socket);
	_clients[new_socket] = client;
}


void	Webserv::getRightServer(Client &client)
{
	bool foundAConf = false;
	Config rightConf = (*(_servers.begin()));

	for (confVector::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		if (!(client.request.getNetwork().hostName.empty()))
		{
			stringVector serverNames = it->getServerName();
			for (stringVector::iterator it2 = serverNames.begin(); it2 != serverNames.end(); it2++)
			{
				if (client.request.getNetwork().hostName == *it2 && it->getNetwork().port == client.request.getNetwork().port)
				{
					foundAConf = true;
					rightConf = *it;
					break ;
				}
			}
		}
		else
		{
			if (client.request.getNetwork() == it->getNetwork())
			{
				rightConf = *it;
				foundAConf = true;
			}
		}
		if (foundAConf)
			break ;
	}
	client.setServer(rightConf);
}

bool	Webserv::read_client_request(int clientSocket)
{
	char client_request[BUFFER_SIZE + 1];
	std::string body("");
	ssize_t ret = recv(clientSocket, client_request, BUFFER_SIZE, 0);

	if (ret == -1)
	{
		removeClient(clientSocket);
		std::cerr << RED << "\rError: recv() failed." << SET << '\n';
		return false;
	}
	else if (ret == 0)
	{
		// std::cout << RED << "\rClosing connection request from clients" << SET << '\n';
		removeClient(clientSocket);
		return false;
	}
	else
	{
		_clients[clientSocket].request.contentSize += ret;
		// client_request[ret] = '\0';
		if (_clients[clientSocket].request.raw_request.empty())
			gettimeofday(&_clients[clientSocket].last_request, NULL);
		_clients[clientSocket].request.raw_request.append(client_request, ret);
	}
	// *********************************************************************************
	if (_clients[clientSocket].request.raw_request.find("\r\n\r\n") != std::string::npos
		&& _clients[clientSocket].request.header_ready == false)
	{
		// std::cout << "**************HEADER READY" << '\n';
		_clients[clientSocket].request.header_ready = true;
		_parser.parseHeader(_clients[clientSocket].request);
		if (std::atoi(_clients[clientSocket].request.getHeader("Content-Length").c_str()) == 0)
		{
			_clients[clientSocket].request.body_ready = true;
			return true;
		}
	}
	if (_clients[clientSocket].request.header_ready == true)
	{
		body = _clients[clientSocket].request.raw_request.substr(_clients[clientSocket].request.raw_request.find("\r\n\r\n") + 4);
		std::string header = _clients[clientSocket].request.raw_request.substr(0, _clients[clientSocket].request.raw_request.find("\r\n\r\n") + 4);


		if ((_clients[clientSocket].request.contentSize - header.size()) == (unsigned long)std::atol(_clients[clientSocket].request.getHeader("Content-Length").c_str()))
		{
			// std::cout << "CONTENT SIZE = " << _clients[clientSocket].request.contentSize - header.size() << " CONTENT LENGTH " << std::atoi(_clients[clientSocket].request.getHeader("Content-Length").c_str()) << '\n';
			_parser.parseBody(_clients[clientSocket].request);
			_clients[clientSocket].request.body_ready = true;
		}
	}
	return true;
}

/*
	Receive a client request and stores it in a string
*/
void Webserv::handleRead(int client_fd)
{
	// timeout
	if (_clients[client_fd].hadResponse == true)
		_clients[client_fd].hadResponse = false;
	if (read_client_request(client_fd) == false)
		return ;
	if (_clients[client_fd].request.body_ready == true)
	{
		// parse the body and set client to EPOLLOUT
		_event.events = EPOLLOUT;
		_event.data.fd = client_fd;
		epoll_ctl(_epfd, EPOLL_CTL_MOD, client_fd, &_event);
	}
}

std::string	get_time_diff(struct timeval *last)
{
	if (last == 0)
		return "        - ";
	struct timeval end;
	gettimeofday(&end, NULL);

	size_t	usec = end.tv_usec - last->tv_usec;
	size_t	sec = end.tv_sec - last->tv_sec;
	bool	is_micro = false;
	std::stringstream ss;

	if (sec > 0)
	{
		ss << sec << " s";
	}
	else
	{
		if (usec >= 1000)
		{
			ss << usec / 1000 << " ms";
		}
		else
		{
			ss << usec << " μs";
			is_micro = true;
		}
	}

	std::string str = ss.str();
	if (str.size() > 10)
		return "eternity..";
	return str.insert(0, (is_micro ? 11 : 10) - str.length(), ' ');
}

void displayInfo(Client &client, Request &request)
{
	char			buffer[100];
	struct tm		*tm;
	std::string path = client.request.getPath();

	// request
	tm = gmtime(&client.last_request.tv_sec);
	strftime(buffer, 100, "%F - %T", tm);

	std::cout << "\r                          \n";
	std::cout << PURPLEBG <<"\r👥[CLIENT]" << SET << "    " << BLUE << buffer << SET << " | ";
	if (path.size() + client.request.getMethod().size() >= 40)
		std::cout << YELLOW << std::left << std::setw(37) << client.request.getMethod() + " " + path.substr(0, 37 - client.request.getMethod().size()) << "...";
	else
		std::cout  << YELLOW << std::left << std::setw(40) << client.request.getMethod() + " " + client.request.getPath();
	std::cout << SET << " " << RED << get_time_diff(&client.last_request) << SET << " | "
	<< GREEN << client.request.getNetwork() << SET << std::endl;
	if (request.contentSize > 900)
		std::cout << request.raw_request.substr(0, 897) << "..." << std::endl;
	else
		std::cout << request.raw_request << std::endl;
}

void displayInfo(Client &client, Response &response)
{
	char			buffer[100];
	struct tm		*tm;
	std::string	resp = response.getResponse();
	std::string firstLine = resp.substr(0, resp.find_first_of('\n') - 1);

	// request
	tm = gmtime(&client.last_request.tv_sec);
	strftime(buffer, 100, "%F - %T", tm);

	std::cout << CYANBG <<"\r🖥️ [WEBSERVER]" << SET << " " << BLUE << buffer << SET << " | "
	 << YELLOW << std::left << std::setw(40) << firstLine << SET;
	std::cout << " " << RED << get_time_diff(&client.last_request) << SET << " | "
	<< GREEN << client.request.getNetwork() << SET << std::endl;
	if (response.getResponse().size() > 900)
		std::cout << response.getResponse().substr(0, 897) << "..." << std::endl;
	else
		std::cout << response.getResponse() << std::endl;

}


void Webserv::handleWrite(int client_fd)
{
	std::string	response;

	// std::cout << "RAW REQUEST" << '\n';
	// std::cout << _clients[client_fd].request.raw_request << '\n';
	getRightServer(_clients[client_fd]);

	_parser.parseResponse(_clients[client_fd].configResponse, _clients[client_fd].request, _clients[client_fd].getServer());
	_clients[client_fd].classResponse.resetResponse(_clients[client_fd].configResponse);
	_clients[client_fd].classResponse.InitResponseProcess();
	if (_clients[client_fd].classResponse.getStatus() == false)
		return ;
	response = _clients[client_fd].classResponse.getResponse();
	// verify in response if we need to use a cgi binary for the request
	//  if so we will pass in a function to execute it
	// ***************************************************
	// std::cout << "*******************RESPONSE" << '\n';
	// std::cout << response << std::endl;
	// Send response
	if(send(client_fd, response.c_str(), response.size(), 0) < 0)
		removeClient(client_fd);
	// timeout
	_clients[client_fd].hadResponse = true;
	displayInfo(_clients[client_fd], _clients[client_fd].request);
	displayInfo(_clients[client_fd], _clients[client_fd].classResponse);
	// listen client again for other requests and wait for a close connection request
	_event.events = EPOLLIN;
	_event.data.fd = client_fd;
	epoll_ctl(_epfd, EPOLL_CTL_MOD, client_fd, &_event);
	// Reset all request variables to reuse it if another request comes in
	_clients[client_fd].request.resetRequest();
}

void Webserv::handleError(int socket)
{
	if (fd_is_server(socket))
	{
		closeServersSocket();
		throw std::logic_error("Error: epoll: fatal error on a server stopping the server");
	}
	epoll_ctl(_epfd, EPOLL_CTL_DEL, socket, NULL);
	close(socket);
}

void Webserv::removeClient(int socket)
{
	_clients.erase(socket);
	epoll_ctl(_epfd, EPOLL_CTL_DEL, socket, NULL);
	close(socket);
}

bool Webserv::check_timeout(struct timeval last)
{
	struct timeval now;
	gettimeofday(&now, NULL);

	if ((now.tv_sec - last.tv_sec) >= CLIENT_TIMEOUT)
		return true;
	return false;
}

void Webserv::handle_timeout_clients(void)
{
	for (mapClients::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (check_timeout((*it).second.last_request))
		{
			// removeClient((*it).first);
			// return handle_timeout_clients();
			if ((*it).second.request.raw_request.empty() || (*it).second.hadResponse == true)
			{
				removeClient((*it).first);
				return handle_timeout_clients();
			}
			else
			{
				(*it).second.request.setRet(408);
				_event.events = EPOLLOUT;
				_event.data.fd = (*it).first;
				epoll_ctl(_epfd, EPOLL_CTL_MOD, (*it).first, &_event);
			}
		}
	}
}

void Webserv::closeServersSocket()
{
	for (fdVector::iterator it = _servers_fd.begin(); it != _servers_fd.end(); it++)
		close(*it);
	close(_epfd);
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
	std::string	request;

	init();
	while (g_run)
	{
		errno = 0;

		nfds = epoll_wait(_epfd, _events_pool, MAX_EV, timeout);
		if (errno == EINVAL || errno == EFAULT || errno == EBADFD)
			std::cerr << "Error: epoll_wait() failed: " << strerror(errno) << '\n';
		else if (errno == EINTR) // epoll interrupted by a signal (CTRL+C)
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
			handle_timeout_clients();
		}
	}
	closeServersSocket();
	std::cout << "\r" << YELLOW << "Shutting down server...       " << SET << std::endl;
}
