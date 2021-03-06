/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_network.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 15:46:53 by vdescham          #+#    #+#             */
/*   Updated: 2022/01/17 18:35:16 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "t_network.hpp"
# include <string>

t_network::t_network(void): hostName("") {}

t_network::~t_network(void) {}

t_network::t_network(t_network const & rhs)
{
	*this = rhs;
}

t_network & t_network::operator=(const t_network &rhs)
{
	if (this != &rhs)
	{
		this->host = rhs.host;
		this->port = rhs.port;
		this->hostName = rhs.hostName;
	}
	return (*this);

}

bool t_network::operator==(t_network const &rhs)
{
	std::string first_ip(inet_ntoa(this->host));
	std::string second_ip(inet_ntoa(rhs.host));
	return ((first_ip == second_ip || (this->host.s_addr == 0 || rhs.host.s_addr == 0)) && this->port == rhs.port);
}

bool t_network::operator!=(t_network const &rhs)
{
	return (!(*this == rhs));
}

std::ostream	&operator<<(std::ostream &out, const t_network &net)
{
	if (net.hostName.empty())
		out << "host: " << inet_ntoa(net.host) << " port: " << net.port;
	else
		out << "host: " << net.hostName << " port: " << net.port;
	return out;
}
