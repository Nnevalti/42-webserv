/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 15:32:32 by sgah              #+#    #+#             */
/*   Updated: 2021/11/22 16:32:01 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser(void) {}

Parser::Parser(Parser const & src)
{
	_configfile = src._configfile;
}

Parser::~Parser(void) {}

void	Parser::config(const char *file)
{

}
