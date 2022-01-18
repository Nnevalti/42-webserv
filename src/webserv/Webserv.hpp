/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 19:53:35 by sgah              #+#    #+#             */
/*   Updated: 2022/01/18 03:28:19 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <cstddef>

# include "t_network.hpp"
# include "Parser.hpp"
# include "Client.hpp"
# include "Request.hpp"
# include "Response.hpp"

class Webserv
{

	public:
		Webserv(void);
		Webserv(confVector configServer);
		~Webserv(void);

		typedef std::set<int>		setPort;
		typedef std::vector<int>	fdVector;
		typedef std::map<int, Client>	mapClients;

		void	setParser(Parser& parser);

		void	run(void);

	private:
		Webserv(Webserv const & src);
		Webserv &	operator=(const Webserv &rhs);

		Parser					_parser;
		// Server config and fds
		confVector				_servers;
		fdVector				_servers_fd;

		// Clients
		mapClients				_clients;
		Client					_tmpClient;

		// epoll variables
		int						_epfd;
		struct epoll_event		_event;
		struct epoll_event		_events_pool[MAX_EV];

		/*
		**	INITILISATION
		*/
		void			init(void);
		void			epollCreate(void);
		bool			is_already_launch(confVector::iterator it_v);
		void			initServers(void);
		int				init_socket(t_network network);
		void			epollInit(void);

		bool			fd_is_server(int ready_fd);
		void			accept_new_client(int server);
		bool			read_client_request(int client_Socket);

		void			getRightServer(Client &client);


		void handleWrite(int client_fd);
		void handleRead(int client_fd);
		void handleError(int client_fd);

		void removeClient(int socket);
		void handle_timeout_clients(void);
		bool check_timeout(struct timeval last);

		void closeServersSocket(void);
};


#endif
