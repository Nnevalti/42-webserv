/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 15:32:32 by sgah              #+#    #+#             */
/*   Updated: 2021/11/22 18:53:38 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser(void) {}

Parser::Parser(Parser const & src)
{
	_configfile = src._configfile;
}

Parser::~Parser(void) {}

void		Parser::readconf(const char *file)
{
	std::string	configfile("");
	std::string	whitespace(" \t\n\v\r\f");
	std::string	tmp;
	char		stack[BUFFER_SIZE + 1];
	int			fd;
	int			ret;
	size_t		start;
	size_t		end(0);

	if ((fd = open(file, O_RDONLY)) <= 0)
		throw std::runtime_error("File does not exist...");

	while ((ret = read(fd, stack, BUFFER_SIZE)) > 0 && !(stack[ret] = '\0'))
		configfile += stack;

	if (ret < 0)
		throw std::runtime_error("Error meanwhile reading the file...");

	configfile += " ";

	while ((start = configfile.find_first_not_of(whitespace, end)) != std::string::npos &&
			(end = configfile.find_first_of(whitespace, start)) != std::string::npos)
	{
		tmp = configfile.substr(start, end - start);
		_configfile.push_back(tmp);
	}
}
