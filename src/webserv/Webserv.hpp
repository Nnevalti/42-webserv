/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 19:53:35 by sgah              #+#    #+#             */
/*   Updated: 2021/11/30 20:35:06 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <cstddef>

# include "t_network.hpp"
# include "Parser.hpp"
# include "Client.hpp"
# include "Request.hpp"

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

		void			initServers(confVector configServer);
		void			verifConfig(void);
		int				init_socket(t_network network);
		void			epoll_init(void);
		int				fd_is_server(int ready_fd);
		void			accept_new_client(int server);
		void			read_client_request(int client_Socket, std::string &request);

		void			getRightServer(Client &client);

};


#endif
