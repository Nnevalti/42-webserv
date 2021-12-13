/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 15:08:27 by sgah              #+#    #+#             */
/*   Updated: 2021/12/13 19:30:40 by sgah             ###   ########.fr       */
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
	if (ac == 2)
	{
		Parser	parse;
		Webserv webserv;

		signal(SIGINT, signal_handler);
		try
		{
			parse.readConf(av[1]);
			parse.readConf();
			parse.parseConf();
			webserv.setParser(parse);
			webserv.run();
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	return 0;
}
