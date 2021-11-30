/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 15:08:27 by sgah              #+#    #+#             */
/*   Updated: 2021/11/29 17:05:59 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Webserv.hpp"

extern volatile bool g_run;

void signal_handler(int signum)
{
	(void)signum;
	g_run = false;
}

/**========================================================================
 **                           MAIN
 *?   This is the starting point of webserv
 *@param ac int  number of parameters
 *@param av char**  tab of parameters
 *@return int
 *========================================================================**/
int		main(int ac, char **av)
{
	Parser	parse;
	Webserv webserv;

	if (ac == 2)
	{
		signal(SIGINT, signal_handler);
		try
		{
			parse.readConf(av[1]);
			parse.parseConf();
			webserv.run(parse.getConfigServers());
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
}
