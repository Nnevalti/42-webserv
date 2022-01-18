/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 15:32:32 by sgah              #+#    #+#             */
/*   Updated: 2022/01/18 14:19:30 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

/**========================================================================
 **                           READCONF
 *?  This function read the configuration file
 *?  before to split it into string vector
 *@param file const char*
 *@return void
 *========================================================================**/
void			Parser::readConf(const char *file)
{
	std::string	conf("");
	std::string	whitespace(" \t\n\v\r\f");
	char		stack[BUFFER_SIZE + 1];
	int			fd;
	int			ret;
	size_t		start;
	size_t		end(0);

	if ((fd = open(file, O_RDONLY)) <= 0)
		throw std::runtime_error("Error Parsing: File does not exist...");

	while ((ret = read(fd, stack, BUFFER_SIZE)) > 0 && !(stack[ret] = '\0'))
		conf += stack;

	close(fd);

	if (ret < 0)
		throw std::runtime_error("Error Parsing: Error meanwhile reading the file...");

	conf += "\n";

	while ((start = conf.find_first_not_of(whitespace, end)) != std::string::npos &&
		(end = conf.find_first_of(whitespace, start)) != std::string::npos)
		_configfile.push_back(conf.substr(start, end - start));

	return ;
}

void			Parser::readConf()
{
	std::string	conf("");
	std::string	whitespace(" \t\n\v\r\f");
	char		stack[BUFFER_SIZE + 1];
	int			fd;
	int			ret;
	size_t		start;
	size_t		end(0);

	if ((fd = open(DEFAULT, O_RDONLY)) <= 0)
		throw std::runtime_error("Error Parsing: File does not exist...");

	while ((ret = read(fd, stack, BUFFER_SIZE)) > 0 && !(stack[ret] = '\0'))
		conf += stack;

	if (ret < 0)
		throw std::runtime_error("Error Parsing: Error meanwhile reading the file...");

	conf += "\n";

	while ((start = conf.find_first_not_of(whitespace, end)) != std::string::npos &&
		(end = conf.find_first_of(whitespace, start)) != std::string::npos)
		_defaultConfigFile.push_back(conf.substr(start, end - start));

	return ;
}

void			Parser::checkDirective(const char* expect, stringVector::iterator* actual)
{
	std::string errormsg;

	errormsg += "Error parsing: directive expected '";
	errormsg += expect;
	errormsg += "' and we got: '";
	errormsg += **actual;
	errormsg += "' instead.";

	if (**actual != expect)
		throw std::runtime_error(errormsg);

	(*actual)++;
}

bool checkServerName(stringVector s1, stringVector s2)
{
	for (stringVector::iterator it = s1.begin(); it != s1.end(); it++)
	{
		for (stringVector::iterator it2 = s2.begin(); it2 != s2.end(); it2++)
		{
			if (*it == *it2)
				return true;
		}
	}
	return false;
}

void Parser::checkConfig(void)
{
	for (confVector::iterator it = _ConfigServers.begin(); it != _ConfigServers.end(); it++)
	{
		t_network net = it->getNetwork();
		std::string ip1 = inet_ntoa(net.host);
		stringVector serverNames = it->getServerName();

		for (confVector::iterator it2 = (it + 1); it2 != _ConfigServers.end(); it2++)
		{
			t_network net2 = it2->getNetwork();
			std::string ip2 = inet_ntoa(net2.host);
			stringVector serverNames2 = it2->getServerName();

			if (net2.port == net.port && ip1 != ip2)
				throw std::runtime_error("Error config: Same port defined multiple time with different IP");
			else if (net2 == net && checkServerName(serverNames, serverNames2))
				throw std::runtime_error("Error: Same IP/Port and server names forbidden");
		}
	}
}

void			Parser::parseConf(void)
{
	for (stringVector::iterator it = _configfile.begin(); it != _configfile.end(); it++)
	{
		Config	confServer;

		for (stringVector::iterator i = _defaultConfigFile.begin(); i != _defaultConfigFile.end(); i++)
		{
			checkDirective("server", &i);
			checkDirective("{", &i);
			parseServer(&i, confServer);
			if (*i != "}")
				throw std::runtime_error("Error Parsing: Curly brackets not close at the end of server block in default.conf");
		}

		checkDirective("server", &it);
		checkDirective("{", &it);
		parseServer(&it, confServer);
		if (*it != "}")
			throw std::runtime_error("Error Parsing: Curly brackets not close at the end of server block");
		_ConfigServers.push_back(confServer);
	}
	checkConfig();
	return ;
}

void			Parser::parseServer(stringVector::iterator* it, Config& server)
{
	stringVector	options;

	while ((*it) != _configfile.end() && (**it) != "}")
	{
		if ((Parser::_ParsingMap.find(**it) != Parser::_ParsingMap.end()) && (**it) != "location")
		{
			std::string directive(**it);

			(*it)++;
			for (;Parser::_ParsingMap.find(**it) == Parser::_ParsingMap.end() && (**it) != "}" && (**it) != "{"; (*it)++)
				options.push_back(**it);
			if ((**it) == "{")
				throw std::runtime_error("Error Parsing: Curly brackets misplace after " + directive);
			(this->*Parser::_ParsingMap[directive])(server, options);
			options.clear();
		}
		else if ((**it) == "location" )
		{
				Config		location;
				std::string	location_name;

				(*it)++;
				if ((**it) == "{" || (**it) == "}")
					throw std::runtime_error("Error Parsing: Curly brackets misplace after location directive");

				for (stringVector::iterator i = _defaultConfigFile.begin(); i != _defaultConfigFile.end(); i++)
				{
					checkDirective("server", &i);
					checkDirective("{", &i);
					parseServer(&i, location);
					if (*i != "}")
						throw std::runtime_error("Error Parsing: Curly brackets not close at the end of server block in default.conf");
				}

				location_name = (**it);
				(*it)++;
				checkDirective("{", it);
				parseServer(it, location);
				checkDirective("}", it);
				server.setLocation(location_name, location);
		}
		else if (Parser::_ParsingMap.find(**it) == Parser::_ParsingMap.end())
		{
			std::string error("Error Parsing: Unknown Directive " + **it);
			throw std::runtime_error(error.c_str());
		}
	}
	return ;
}

parseDirectiveMap	Parser::_initParsingMap()
{
	parseDirectiveMap	parseMap;
	parseMap["server_name"] = &Parser::parseServerName;
	parseMap["listen"] = &Parser::parseNetwork;
	parseMap["root"] = &Parser::parseRoot;
	parseMap["error_page"] = &Parser::parseErrorPage;
	parseMap["client_body_buffer_size"] = &Parser::parseClientBodyBufferSize;
	parseMap["cgi_pass"] = &Parser::parseCgiPass;
	parseMap["allow_methods"] = &Parser::parseAllowedMethods;
	parseMap["index"] = &Parser::parseIndex;
	parseMap["autoindex"] = &Parser::parseAutoIndex;
	parseMap["location"] = NULL;
	parseMap["alias"] = &Parser::parseAlias;
	parseMap["return"] = &Parser::parseReturn;
	return parseMap;
}

parseDirectiveMap	Parser::_ParsingMap = Parser::_initParsingMap();

void			Parser::parseReturn(Config& configServer, stringVector opts)
{
	if (opts.size() < 2)
		throw std::runtime_error("Error Parsing: Missing Return's directive arguments");
	if (opts.size() > 2)
		throw std::runtime_error("Error Parsing: Too much Return's directive arguments");
	configServer.setReturn(opts[0], opts[1]);
}


void			Parser::parseRoot(Config& configServer,stringVector opts)
{
	if (opts.empty())
		throw std::runtime_error("Error Parsing: Missing root's directive argument");
	if (opts.size() > 1)
		throw std::runtime_error("Error Parsing: Too much root's directive arguments");
	configServer.setRoot(opts.front());
}

void			Parser::parseAlias(Config& configServer,stringVector opts)
{
	if (opts.empty())
		throw std::runtime_error("Error Parsing: Missing alias's directive argument");
	else if (opts.size() > 1)
		throw std::runtime_error("Error Parsing: Too much alias's directive arguments");
	configServer.setAlias(opts.front());
}

void			Parser::parseIndex(Config& configServer,stringVector opts)
{
	if (opts.empty())
		throw std::runtime_error("Error Parsing: Missing Index's directive arguments");
	if (opts.size() > 2)
		throw std::runtime_error("Error Parsing: Too much Index's directive arguments");
	configServer.setIndex(opts.front());
}

void			Parser::parseNetwork(Config& configServer,stringVector opts)
{
	t_network	net;
	size_t		colons;
	size_t		point;
	std::string	host;

	if (opts.size() > 1)
		throw std::runtime_error("Error Parsing: Too much listen's directive arguments");
	if (opts.empty())
		throw std::runtime_error("Error Parsing: Missing listen's directive arguments");

	std::string address(opts.front());

	//* PORT ONLY
	if (((colons = address.find(":")) == std::string::npos) && ((point = address.find(".")) == std::string::npos) && address != "localhost")
	{
		net.host.s_addr = 0;
		net.port = std::atoi(address.c_str());
		configServer.setNetwork(net);
		return ;
	}
	//* HOST ONLY
	if(colons == std::string::npos && (point != std::string::npos || address == "localhost"))
	{
		if (address == "localhost")
			host = "127.0.0.1";
		else
			host = address;
		net.host.s_addr = inet_addr(host.c_str());
		net.port = 8080;
		configServer.setNetwork(net);
		return ;
	}
	if ((host = address.substr(0, colons)) == "localhost")
		host = "127.0.0.1";
	colons++;
	net.host.s_addr = inet_addr(host.c_str());
	net.port = std::atoi(address.substr(colons).c_str());
	configServer.setNetwork(net);
}

void					Parser::parseCgiPass(Config& configServer,stringVector opts)
{
	if (opts.size() > 1)
		throw std::runtime_error("Error Parsing: Too much cgi_pass's directive arguments");
	if (opts.empty())
		throw std::runtime_error("Error Parsing: Missing cgi_pass's directive arguments");
	configServer.setCgiPass(opts.front());
}

void					Parser::parseErrorPage(Config& configServer,stringVector opts)
{
	std::vector<int>	error_code;
	int					code;
	std::string			error_page("");
	stringIntVectorMap	error_map(configServer.getErrorPage());

	if(opts.empty())
		throw std::runtime_error("Error Parsing: Missing error_page's directive arguments(root to page)");

	for (stringVector::iterator i = opts.begin(); i != opts.end(); i++)
	{
		if ((code = std::atoi((*i).c_str())) != 0)
			error_code.push_back(code);
		else
			error_page = *i;
	}

	if(error_page == "")
		throw std::runtime_error("Error Parsing: Missing error_page's directive arguments(root to page)");
	if(error_code.empty())
		throw std::runtime_error("Error Parsing: Missing error_page's directive arguments(error code)");

	if(error_map.find(error_page) != error_map.end())
	{
		for (std::vector<int>::iterator i = error_code.begin(); i != error_code.end(); i++)
			configServer.setErrorCode(error_page, *i);
		return ;
	}

	configServer.setErrorPage(error_page, error_code);
}

void					Parser::parseAutoIndex(Config& configServer,stringVector opts)
{
	bool i;
	if (opts.size() > 1)
		throw std::runtime_error("Error Parsing: Too much Auto_index's directive arguments");
	if (opts.empty())
		throw std::runtime_error("Error Parsing: Missing Auto_index's directive arguments");
	if(opts.front() == "on")
		i = true;
	else if (opts.front() == "off")
		i = false;
	else
		throw std::runtime_error("Error Parsing: Auto_index's directive arguments is incorrect");
	configServer.setAutoIndex(i);
}

void					Parser::parseServerName(Config& configServer,stringVector opts)
{
	if (opts.empty())
		throw std::runtime_error("Error Parsing: Missing server_name's directive arguments");
	if (opts.size() > 2)
		throw std::runtime_error("Error Parsing: Too much server_name's directive arguments");
	configServer.setServerName(opts);
}

void					Parser::parseAllowedMethods(Config& configServer,stringVector opts)
{
	if (opts.empty())
		throw std::runtime_error("Error Parsing: Missing allowed_method's directive arguments");

	configServer.setAllowedMethods(opts);
}

void					Parser::parseClientBodyBufferSize(Config& configServer,stringVector opts){
	if (opts.empty())
		throw std::runtime_error("Error Parsing: Missing Client_body_buffer_size's directive arguments");
	if (opts.size() > 1)
		throw std::runtime_error("Error Parsing: too much Client_body_buffer_size's directive arguments");
	configServer.setClientBodyBufferSize(std::atoi(opts.front().c_str()));
}

confVector	Parser::getConfigServers(void)
{
	return (_ConfigServers);
}
