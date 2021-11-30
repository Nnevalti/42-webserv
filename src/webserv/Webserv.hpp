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

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "t_network.hpp"
# include "Config.hpp"

class Webserv
{

	public:
		Webserv(void);
		Webserv(confVector configServer);
		~Webserv(void);

		typedef std::set<int> setPort;
		typedef std::vector<int> fdVector;

		void	run(confVector configServer);

	private:
		Webserv(Webserv const & src);
		Webserv &	operator=(const Webserv &rhs);

		confVector				_servers;
		fdVector				_servers_fd;
		// epoll variables
		int						_epfd;
		struct epoll_event		_event;
		struct epoll_event		_events_pool[MAX_EV];

		void			initServers(confVector configServer);
		int				init_socket(t_network network);
		void			epoll_init(void);
		int				fd_is_server(int ready_fd);
		void			accept_new_client(int server);
		std::string		read_client_request(int client_socket);

};


#endif
