/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_network.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdescham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 15:46:53 by vdescham          #+#    #+#             */
/*   Updated: 2021/11/29 15:46:53 by vdescham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_network.hpp"

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
	}
	return (*this);

}

bool t_network::operator==(t_network const &rhs)
{
	return (inet_ntoa(this->host) == inet_ntoa(rhs.host) && this->port == rhs.port);
}

bool t_network::operator!=(t_network const &rhs)
{
	return (!(*this == rhs));
}
