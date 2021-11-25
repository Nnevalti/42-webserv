/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdescham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 13:49:50 by vdescham          #+#    #+#             */
/*   Updated: 2021/11/25 13:49:50 by vdescham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include <iostream>
# include "headers.hpp"
# include "Config.hpp"

class Server
{
	public:
		Server(void);
		Server(Server const & src);
		~Server(void);

		Server&		operator=(const Server &rhs);
		void		setConfig(Config config);
		Config		getConfig(void) const;
	private:
		Config		_config;
};

#endif
