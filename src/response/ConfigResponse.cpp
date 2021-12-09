/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:28:48 by sgah              #+#    #+#             */
/*   Updated: 2021/12/09 01:39:15 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigResponse.hpp"

ConfigResponse::ConfigResponse(void) {}

ConfigResponse::ConfigResponse(ConfigResponse const & src)
{
	*this = src;
}

ConfigResponse::~ConfigResponse(void) {}

ConfigResponse&		ConfigResponse::operator=(const ConfigResponse& rhs)
{
	_location = rhs._location;
	return (*this);
}

void				ConfigResponse::setServer(Config server)
{
	_server = server;
}


void				ConfigResponse::setLocation(Config location)
{
	_location = location;
}

void				ConfigResponse::setLocationPath(std::string path)
{
	if (*(--path.end()) == '/')
		path.resize(path.size() - 1);

	_locationPath = path;
}

void				ConfigResponse::setLocationFile(std::string path)
{
	_locationFile = path;
}

void				ConfigResponse::setErrorMap(stringIntVectorMap map)
{
	for (stringIntVectorMap::iterator i = map.begin(); i != map.end(); i++)
		for (std::vector<int>::iterator it = i->second.begin(); it != i->second.end(); i++)
			_errorPage[*it] = i->first;
}

void				ConfigResponse::setClientBodyBufferSize(size_t size)
{
	_clientBodyBufferSize = size;
}

void				ConfigResponse::setCgiParam(stringVector cgiParam)
{
	_cgiParam = cgiParam;
}

void				ConfigResponse::setCgiPass(std::string cgiPass)
{
	_cgiPass = cgiPass;
}

void				ConfigResponse::setAllowMethod(stringVector allow)
{
	for (stringVector::const_iterator i = allow.begin(); i != allow.end(); i++)
		_allowMethod.insert(*i);
}

void				ConfigResponse::setLanguage(std::string lang)
{
	_acceptLanguage = lang;
}

void				ConfigResponse::setAutoIndex(bool index)
{
	_autoIndex = index;
}

void				ConfigResponse::setIndex(stringVector index)
{
	for (stringVector::iterator i = index.begin(); i != index.end(); i++)
		if (std::find(_index.begin(), _index.end(), *i) == _index.end())
			_index.push_back(*i);
}

void				ConfigResponse::setContentLocation(std::string content)
{
	_contentLocation = content;
}

Config				ConfigResponse::getServer(void) const
{
	return (_server);
}

Config				ConfigResponse::getLocation(void) const
{
	return (_location);
}

std::string			ConfigResponse::getLocationPath(void) const
{
	return (_locationPath);
}

std::string			ConfigResponse::getLocationFile(void) const
{
	return (_locationFile);
}

errorMap			ConfigResponse::getErrorMap(void) const
{
	return (_errorPage);
}

size_t				ConfigResponse::getClientBodyBufferSize(void) const
{
	return (_clientBodyBufferSize);
}

stringVector		ConfigResponse::getCgiParam(void) const
{
	return (_cgiParam);
}

std::string			ConfigResponse::getCgiPass(void) const
{
	return (_cgiPass);
}

stringSet			ConfigResponse::getAllowMethod(void) const
{
	return (_allowMethod);
}

std::string			ConfigResponse::getLanguage(void) const
{
	return (_acceptLanguage);
}

bool				ConfigResponse::getAutoIndex(void) const
{
	return (_autoIndex);
}

stringVector		ConfigResponse::getIndex(void) const
{
	return (_index);
}

std::string			ConfigResponse::getContentLocation(void) const
{
	return (_contentLocation);
}
