/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 16:16:28 by sgah              #+#    #+#             */
/*   Updated: 2021/11/29 17:14:27 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config(void): _client_body_buffer_size(0), _autoindex(false)
{}

Config::Config(Config const & src)
{
	*this = src;
}

Config::~Config(void) {}

Config &		Config::operator=(Config const &src)
{

	if (this != &src)
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
	}

	return *this;
}

void		Config::addNetwork(t_network &net)
{
	_network = net;
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

void		Config::addCgiParam(stringVector cgiParam)
{
	_cgi_param = cgiParam;
}

void		Config::addCgiPass(std::string cgiPass)
{
	_cgi_pass = cgiPass;
}

void		Config::addLocation(std::string location_name, Config location)
{
	_location[location_name] = location;
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

void		Config::addErrorPage(std::string page, std::vector<int> codes)
{
	_error_page.insert(std::pair<std::string, std::vector<int> >(page, codes));
}

t_network	Config::getNetwork(void)
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

stringVector	Config::getCgiParam(void)
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

StringIntVectorMap	Config::getErrorPage(void)
{
	return (_error_page);
}

std::ostream	&operator<<(std::ostream &out, const Config &server)
{
	out << "============================================" << std::endl;
	out << "                   CONF" << std::endl;
	out << "============================================" << std::endl;

	out << "Listen:" << std::endl;

	out << "\thost: " << inet_ntoa(server._network.host) << " port: " << server._network.port << std::endl;

	out << "root: " << server._root << std::endl;

	out << "server_name: ";
	for (stringVector::const_iterator i = server._server_name.begin(); i != server._server_name.end(); i++)
	{
		out << *i;
		if (i != server._server_name.end() - 1)
			out << " ";
	}

	out << std::endl<< "error_page:" << std::endl;
	for (StringIntVectorMap::const_iterator i = server._error_page.begin(); i != server._error_page.end(); i++)
	{
		out << "\t";
		std::vector<int> tmp((*i).second);
		for (std::vector<int>::const_iterator it = tmp.begin(); it != tmp.end(); i++)
		{
			out << *it;
			if (it != tmp.end() - 1)
				out << " ";

		}
		out << i->first << std::endl;
	}

	out << "client_body_buffer_size: " << server._client_body_buffer_size << std::endl;

	out << "cgi_param:" << std::endl;
	for (stringVector::const_iterator i = server._cgi_param.begin(); i != server._cgi_param.end(); i++)
		if (i == server._cgi_param.begin())
			out << "\t" << *i << " = ";
		else
			out << *i << std::endl;

	out << "cgi_pass:	" << server._cgi_pass << std::endl;

	out << "allowed methods: ";
	for (stringVector::const_iterator i = server._allowed_methods.begin(); i != server._allowed_methods.end(); i++)
		out << " " << *i;

	out << std::endl;

	out << "autoindex " << (server._autoindex ? "on" : "off") << std::endl;

	out << "index: ";
	for (stringVector::const_iterator i = server._index.begin(); i != server._index.end(); i++)
		out << *i << " ";

	out << std::endl;

	out << "alias: " << server._alias << std::endl;

	for (serverMap::const_iterator i = server._location.begin(); i != server._location.end(); i++)
		out << std::endl << "LOCATION " << i->first << std::endl << i->second << std::endl;

	out << "=============== END OF CONF ==============" << std::endl;
	return out;
}
