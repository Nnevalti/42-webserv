/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:28:48 by sgah              #+#    #+#             */
/*   Updated: 2021/12/09 06:11:36 by sgah             ###   ########.fr       */
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
	_request = rhs._request;
	_location = rhs._location;
	_server = rhs._server;
	_locationPath = rhs._locationPath;
	_locationFile = rhs._locationFile;
	_errorPage = rhs._errorPage;
	_clientBodyBufferSize = rhs._clientBodyBufferSize;
	_cgiParam = rhs._cgiParam;
	_cgiPass = rhs._cgiPass;
	_allowMethod = rhs._allowMethod;
	_acceptLanguage = rhs._acceptLanguage;
	_autoIndex = rhs._autoIndex;
	_index = rhs._index;
	_contentLocation = rhs._contentLocation;
	return (*this);
}

std::ostream	&operator<<(std::ostream &out, const ConfigResponse &conf)
{
	out << "/**============================================\n"
	<< " *            CONFIG RESPONSE\n**=============================================**/\n";
	out << "Location: \n" << conf._location << std::endl;
	out << "Right server: \n" << conf._server << std::endl;
	out << "Location path : " << conf._locationPath << std::endl;
	out << "Location File : " << conf._locationFile << std::endl;

	out << "error_page :" << std::endl;
	for (errorMap::const_iterator i = conf._errorPage.begin(); i != conf._errorPage.end(); i++)
		out << "\t" << i->first << " : " + i->second << std::endl;

	out << "Client body buffer size : " << conf._clientBodyBufferSize << std::endl;

	out << "Cgi param : ";
	for (stringVector::const_iterator i = conf._cgiParam.begin(); i != conf._cgiParam.end(); i++)
		out << *i + " ";

	out << "Allow Method : ";
	for (stringSet::const_iterator i = conf._allowMethod.begin(); i != conf._allowMethod.end(); i++)
		out << *i + " ";
	out << std::endl;

	out << "Accept Language : " << conf._acceptLanguage << std::endl;
	out << "Auto index is :" << ((conf._autoIndex) ? "on" : "off") << std::endl;

	out << "Index : ";
	for (stringVector::const_iterator i = conf._index.begin(); i != conf._index.end(); i++)
		out << *i + " ";

	out << "Content Location: " << conf._contentLocation << std::endl;
	return (out);
}

void				ConfigResponse::setRequest(Request& request)
{
	_request = request;
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

Request				ConfigResponse::getRequest(void) const
{
	return (_request);
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