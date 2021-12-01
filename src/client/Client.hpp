/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdescham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 13:49:50 by vdescham          #+#    #+#             */
/*   Updated: 2021/11/25 13:49:50 by vdescham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include "headers.hpp"
# include "Request.hpp"

class Client
{
	public:
		Client(void);
		Client(Client const & src);
		~Client(void);

		Client&		operator=(const Client &rhs);

		typedef std::list<Request> listReq;

		int			getSocket(void);
		void		setSocket(int socket);
		listReq		getRequests(void);
		void		addRequest(Request request);
		void		removeRequest(listReq::iterator it);

	private:
		int			_socket;
		listReq		_requests;
};

#endif
