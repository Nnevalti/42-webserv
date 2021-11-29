/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 15:32:32 by sgah              #+#    #+#             */
/*   Updated: 2021/11/29 17:07:38 by sgah             ###   ########.fr       */
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

	if (ret < 0)
		throw std::runtime_error("Error Parsing: Error meanwhile reading the file...");

	conf += " ";

	while ((start = conf.find_first_not_of(whitespace, end)) != std::string::npos &&
		(end = conf.find_first_of(whitespace, start)) != std::string::npos)
		_configfile.push_back(conf.substr(start, end - start));

	return ;
}

void			Parser::checkDirective(const char* expect, stringVector::iterator* actual)
{
	std::string errormsg;

	errormsg += "directive expected '";
	errormsg += expect;
	errormsg += "' and we got: '";
	errormsg += **actual;
	errormsg += "' instead.";

	if (**actual != expect)
		throw std::runtime_error(errormsg);

	(*actual)++;
}

void			Parser::parseConf(void)
{
	for (stringVector::iterator it = _configfile.begin(); it != _configfile.end(); it++)
	{
		Config	confServer;
		std::cout << *it << std::endl;
		checkDirective("server", &it);
		checkDirective("{", &it);
		parseServer(&it, confServer);
		if (*it != "}")
			throw std::runtime_error("Error Parsing: Curly brackets not close at the end of server block");
		std::cout << confServer;
		_ConfigServers.push_back(confServer);
	}
	return ;
}

void			Parser::parseServer(stringVector::iterator* it, Config& server)
{
	stringVector	options;

	while ((*it) != _configfile.end() && (**it) != "}")
	{
		if (Parser::_ParsingMap.find(**it) != Parser::_ParsingMap.end() && (**it) != "location")
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
					std::runtime_error("Error Parsing: Curly brackets misplace after location directive");

				location_name = **it;
				(*it) = (*it) + 2;
				parseServer(it, location);
				checkDirective("}", it);
				server.addLocation(location_name, location);
		}
		else /*if ((**it) != "}")*/
			std::runtime_error("Error Parsing: Unknown directive: " + (**it));
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
	parseMap["cgi_param"] = &Parser::parseCgiParam;
	parseMap["cgi_pass"] = &Parser::parseCgiPass;
	parseMap["allow_methods"] = &Parser::parseAllowedMethods;
	parseMap["index"] = &Parser::parseIndex;
	parseMap["autoindex"] = &Parser::parseAutoIndex;
	parseMap["location"] = NULL;
	parseMap["alias"] = &Parser::parseAlias;
	return parseMap;
}

parseDirectiveMap	Parser::_ParsingMap = Parser::_initParsingMap();

void			Parser::parseRoot(Config& configServer,stringVector opts)
{
	if (opts.empty())
		throw std::runtime_error("Error Parsing: Missing root's directive argument");
	if (opts.size() > 1)
		throw std::runtime_error("Error Parsing: Too much root's directive arguments");
	configServer.addRoot(opts.front());
}

void			Parser::parseAlias(Config& configServer,stringVector opts)
{
	if (opts.empty())
		throw std::runtime_error("Error Parsing: Missing alias's directive argument");
	else if (opts.size() > 1)
		throw std::runtime_error("Error Parsing: Too much alias's directive arguments");
	configServer.addAlias(opts.front());
}

void			Parser::parseIndex(Config& configServer,stringVector opts)
{
	if (opts.empty())
		throw std::runtime_error("Error Parsing: Missing Index's directive arguments");
	configServer.addIndex(opts);
}

void			Parser::parseNetwork(Config& configServer,stringVector opts)
{
	t_network	net;
	size_t		i;
	std::string	host;

	if (opts.size() > 1)
		throw std::runtime_error("Error Parsing: Too much listen's directive arguments");
	if (opts.empty())
		throw std::runtime_error("Error Parsing: Missing listen's directive arguments");

	std::string address(opts.front());

	if ((i = address.find(":")) == std::string::npos)
	{
		net.host.s_addr = 0;
		net.port = std::atoi(address.c_str());
		configServer.addNetwork(net);
		return ;
	}
	if ((host = address.substr(0, i)) == "localhost")
		host = "127.0.0.1";

	i++;
	net.host.s_addr = inet_addr(host.c_str());
	net.port = std::atoi(address.substr(i).c_str());
	configServer.addNetwork(net);
}

void					Parser::parseCgiPass(Config& configServer,stringVector opts)
{
	if (opts.size() > 1)
		throw std::runtime_error("Error Parsing: Too much cgi_pass's directive arguments");
	if (opts.empty())
		throw std::runtime_error("Error Parsing: Missing cgi_pass's directive arguments");
	configServer.addCgiPass(opts.front());
}

void					Parser::parseCgiParam(Config& configServer,stringVector opts)
{

	if (opts.size() > 2)
		throw std::runtime_error("Error Parsing: Too much cgi_param's directive arguments");
	if (opts.size() <= 1)
		throw std::runtime_error("Error Parsing: Missing cgi_param's directive arguments");
	configServer.addCgiParam(opts);
}

void					Parser::parseErrorPage(Config& configServer,stringVector opts)
{
	std::vector<int>	error_code;
	int					code;
	std::string			error_page("");

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
	configServer.addErrorPage(error_page, error_code);
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
	configServer.addAutoIndex(i);
}

void					Parser::parseServerName(Config& configServer,stringVector opts)
{
	if (opts.empty())
		throw std::runtime_error("Error Parsing: Missing server_name's directive arguments");
	configServer.addServerName(opts);
}

void					Parser::parseAllowedMethods(Config& configServer,stringVector opts)
{
	if (opts.empty())
		throw std::runtime_error("Error Parsing: Missing allowed_method's directive arguments");

	configServer.addAllowedMethods(opts);
}

void					Parser::parseClientBodyBufferSize(Config& configServer,stringVector opts){
	if (opts.empty())
		throw std::runtime_error("Error Parsing: Missing Client_body_buffer_size's directive arguments");
	configServer.addClientBodyBufferSize(std::atoi(opts.front().c_str()));
}

confVector	Parser::getConfigServers(void)
{
	return (_ConfigServers);
}
