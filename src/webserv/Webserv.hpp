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
		Webserv(Webserv const & src);
		~Webserv(void);
		Webserv &	operator=(const Webserv &rhs);

		// typedef std::set<int> setPort;
		// typedef std::vector<std::pair<std::string, int> > vectorPorts;
		// typedef std::vector<int> fd_vector;
		typedef std::vector<Server> serverVector;

		void buildServers(confVector configServer);
	private:
		serverVector	_servers;
		// setPort		_listeningPorts;

};


#endif
