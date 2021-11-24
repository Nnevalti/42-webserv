/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 16:16:28 by sgah              #+#    #+#             */
/*   Updated: 2021/11/24 05:51:52 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config(void) {}

Config::Config(Config const & src)
{
	*this = src;
}

Config::~Config(void) {}

Config &		Config::operator=(Config const &src)
{

	_network = src._network;
	_root = src._root;
	_server_name = src._server_name;
	_error_page = src._error_page;
	_client_body_buffer_size = src._client_body_buffer_size;
	_cgi_param = src._cgi_param;
	_cgi_pass = src._cgi_pass;
	_location = src._location;
	_allowed_methods = src._allowed_methods;
	_autoindex = src._autoindex;
	_index = src._index;
	_alias = src._alias;
	_alias_set = src._alias_set;

	return *this;
}

void		Config::addNetwork(t_network net)
{
	_network.push_back(net);
}

void		Config::addServerName(stringVector Name)
{
	_server_name = Name;
}

void		Config::addRoot(std::string root)
{
	_root = root;
}

void		Config::addClientBodyBufferSize(int buffer)
{
	_client_body_buffer_size = buffer;
}

void		Config::addCgiParam(stringMap cgiParam)
{
	_cgi_param = cgiParam;
}

void		Config::addCgiPass(std::string cgiPass)
{
	_cgi_pass = cgiPass;
}

void		Config::addLocation(serverMap location)
{
	_location = location;
}

void		Config::addAllowedMethods(stringVector methods)
{
	_allowed_methods = methods;
}

void		Config::addIndex(stringVector index)
{
	_index = index;
}

void		Config::addAutoIndex(bool autoIndex)
{
	_autoindex = autoIndex;
}

void		Config::addAlias(std::string alias)
{
	_alias = alias;
	_alias_set = true;
}

netVector		Config::getNetwork(void)
{
	return (_network);
}

stringVector	Config::getServerName(void)
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

std::ostream	&operator<<(std::ostream &out, const Config &server)
{
	out << "root: " << server._root << std::endl;
	return (out);
}
