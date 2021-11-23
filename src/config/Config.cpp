/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 16:16:28 by sgah              #+#    #+#             */
/*   Updated: 2021/11/24 00:01:40 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config(void) {}

Config::Config(Config const & src)
{
	(void)src;
}

Config::~Config(void) {}

netVector		Config::getNetwork(void)
{
	return (_network);
}

stringVector	Config::getServerNAme(void)
{
	return (_server_name);
}

std::string		Config::getRoot(void)
{
	return (_root);
}

int				Config::getClientBodyBufferSize(void)
{
	return (_client_body_buffer_size);
}

stringMap		Config::getCgiParam(void)
{
	return (_cgi_param);
}

std::string		Config::getCgiPass(void)
{
	return (_cgi_pass);
}

serverMap		Config::getLocation(void)
{
	return (_location);
}

stringVector	Config::getAllowedMethods(void)
{
	return (_allowed_methods);
}

stringVector	Config::getIndex(void)
{
	return(_index);
}

bool			Config::getAutoIndex(void)
{
	return (_autoindex);
}

std::string		Config::getAlias(void)
{
	return (_alias);
}

