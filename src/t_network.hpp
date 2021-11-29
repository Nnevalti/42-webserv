/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_network.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdescham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 15:46:53 by vdescham          #+#    #+#             */
/*   Updated: 2021/11/29 15:46:53 by vdescham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_NETWORK_HPP
# define T_NETWORK_HPP

# include <netinet/in.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>


class t_network
{
	public:
		t_network(void);
		t_network(t_network const & src);
		t_network &	operator=(const t_network &rhs);
		~t_network(void);
		bool operator==(t_network const &rhs);
		bool operator!=(t_network const &rhs);

		struct in_addr	host;
		int				port;

	private:

};


#endif
