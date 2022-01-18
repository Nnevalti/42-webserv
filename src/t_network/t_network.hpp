/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_network.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 15:46:53 by vdescham          #+#    #+#             */
/*   Updated: 2022/01/18 03:27:04 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_NETWORK_HPP
# define T_NETWORK_HPP

# include <netinet/in.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <ostream>

class t_network
{
	public:
		t_network(void);
		t_network(t_network const & src);
		t_network &	operator=(const t_network &rhs);
		~t_network(void);
		bool operator==(t_network const &rhs);
		bool operator!=(t_network const &rhs);
		friend std::ostream	&operator<<(std::ostream &out, const t_network &net);

		struct in_addr	host;
		int				port;
		std::string		hostName; // for request only

	private:

};

#endif
