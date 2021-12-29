/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 13:49:50 by vdescham          #+#    #+#             */
/*   Updated: 2021/12/06 20:36:05 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include "headers.hpp"
# include "Request.hpp"
# include "Config.hpp"

class Client
{
	public:
		Client(void);
		Client(Client const & src);
		~Client(void);

		Client&		operator=(const Client &rhs);

		typedef std::list<Request> listReq;

		void		setSocket(int socket);
		int			getSocket(void);
		std::string	&getRequest(void);

		void		setServer(Config server);
		Config&		getServer(void);

		int		readRequest(void);
		void		sendResponse(void);

		// request class we get from parsing
		Request		request;
		// handle timeout
		struct timeval last_request;

	private:
		int			_socket;
		std::string _request;

		Config		_server;
};

#endif
