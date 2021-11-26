/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 19:53:35 by sgah              #+#    #+#             */
/*   Updated: 2021/11/22 19:54:38 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_H
# define WEBSERV_H

# include "headers.hpp"
# include "Config.hpp"
# include "Server.hpp"

class Webserv
{

	public:
		Webserv(void);
		~Webserv(void);

		typedef std::set<int> setPort;
		// typedef std::vector<std::pair<std::string, int> > vectorPorts;
		// typedef std::vector<int> fd_vector;
		typedef std::vector<Server> serverVector;
		typedef std::vector<int> fd_vector;

		void buildServers(confVector configServer);
		int		run(void);

	private:
		Webserv(Webserv const & src);
		Webserv &	operator=(const Webserv &rhs);
		serverVector	_servers;
		setPort			_listeningPorts;
		fd_vector		_servers_fd;
		int				_epfd;
		struct epoll_event _event;
		struct epoll_event _events_pool[MAX_EV];


		int		init_socket(int port);
		void	epoll_init(void);
		int		fd_is_server(int ready_fd);
};


#endif
