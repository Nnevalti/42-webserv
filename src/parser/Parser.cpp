/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 20:07:42 by sgah              #+#    #+#             */
/*   Updated: 2021/11/30 20:08:08 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

/**========================================================================
 *                           CONSTRUCTOR/DESTRUCTOR
 *========================================================================**/
Parser::Parser(void) {}

Parser::Parser(Parser const & src)
{
	_configfile = src._configfile;
}

Parser::~Parser(void) {}

/**========================================================================
 *                           OVERLOAD OPERATOR =
 *========================================================================**/
Parser&			Parser::operator=(const Parser &rhs)
{
	this->_configfile = rhs._configfile;
	this->_ConfigServers = rhs._ConfigServers;
	return (*this);
}
