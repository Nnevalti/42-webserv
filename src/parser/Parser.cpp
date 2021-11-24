/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 15:32:32 by sgah              #+#    #+#             */
/*   Updated: 2021/11/24 06:09:12 by sgah             ###   ########.fr       */
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
		throw std::runtime_error("File does not exist...");

	while ((ret = read(fd, stack, BUFFER_SIZE)) > 0 && !(stack[ret] = '\0'))
		conf += stack;

	if (ret < 0)
		throw std::runtime_error("Error meanwhile reading the file...");

	conf += " ";

	while ((start = conf.find_first_not_of(whitespace, end)) != std::string::npos &&
		(end = conf.find_first_of(whitespace, start)) != std::string::npos)
		_configfile.push_back(conf.substr(start, end - start));

	return ;
}

int				Parser::checkDirective(const char* expect, stringVector::iterator* actual)
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
	return (1);
}

void			Parser::parseConf(void)
{
	for (stringVector::iterator it = _configfile.begin(); it != _configfile.end(); it++)
		if (checkDirective("server", &it))
		{
			if (checkDirective("{", &it))
			{
				Config	confServer;

				parseServer(&it, confServer);
				std::cout << confServer;
				_ConfigServers.push_back(confServer);
			}
		}
	return ;
}

void			Parser::parseServer(stringVector::iterator* it, Config& server)
{
	stringVector	options;
	std::string		directive;

	while ((*it) != _configfile.end() && (**it) != "}")
	{
		if (Parser::_ParsingMap.find(**it) != Parser::_ParsingMap.end() && (**it) != "location")
		{
			directive = **it;
			(*it)++;
			for (;Parser::_ParsingMap.find(**it) == Parser::_ParsingMap.end() && (**it) != "}"; (*it)++)
				options.push_back(**it);
			(this->*Parser::_ParsingMap[directive])(server, options);
			std::cout << directive << options.front() << std::endl;
		}
	}
}

addDirectiveMap	Parser::_initParsingMap()
{
	addDirectiveMap	parseMap;
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
	return parseMap;
}

addDirectiveMap	Parser::_ParsingMap = Parser::_initParsingMap();

void			Parser::parseRoot(Config& configServer,stringVector opts)
{
	if (opts.empty())
		throw std::runtime_error("Missing root's directive argument");
	configServer.addRoot(opts.front());
}

void			Parser::parseAlias(Config& configServer,stringVector opts)
{
	if (opts.empty())
		throw std::runtime_error("Missing alias's directive argument");
	else if (opts.size() > 1)
		throw std::runtime_error("Too much alias's directive arguments");
	configServer.addAlias(opts.front());
}
void			Parser::parseIndex(Config& configServer,stringVector opts)
{
	if (opts.empty())
		throw std::runtime_error("Missing Index's directive arguments");
	configServer.addIndex(opts);
}
void			Parser::parseNetwork(Config& configServer,stringVector opts)
{
	t_network	net;
	size_t		i;
	std::string	host;

	if (opts.size() > 1)
		throw std::runtime_error("Too much listen's directive arguments");
	if (opts.size == 0)
		throw std::runtime_error("Missing listen's directive arguments");
	if ((i = opts.front().find(":") == std::string::npos))
	{
		net.host = 0;
		net.port = std::atoi(opts.front());
		configServer.addNetwork(net);
		return ;
	}
	if ((host = opts.front().substr(0, i++)) == "localhost")
		host = "127.0.0.1";

	net.host = inet_addr(host);
	net.port = std::atoi(opts.front().substr(i));
	configServer.addNetwork(net);
}
void					Parser::parseCgiPass(Config& configServer,stringVector opts)
{
	(void)configServer;
	(void) opts;
}
void					Parser::parseCgiParam(Config& configServer,stringVector opts)
{
	(void)configServer;
	(void) opts;
}
void					Parser::parseErrorPage(Config& configServer,stringVector opts)
{
	(void)configServer;
	(void) opts;
}
void					Parser::parseAutoIndex(Config& configServer,stringVector opts)
{
	(void)configServer;
	(void) opts;
}
void					Parser::parseServerName(Config& configServer,stringVector opts)
{
	(void)configServer;
	(void) opts;
}
void					Parser::parseAllowedMethods(Config& configServer,stringVector opts)
{
	(void)configServer;
	(void) opts;
}
void					Parser::parseClientBodyBufferSize(Config& configServer,stringVector opts){
	(void)configServer;
	(void) opts;
}


confVector	Parser::getConfigServers(void)
{
	return (_ConfigServers);
}
