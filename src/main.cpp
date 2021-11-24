/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 15:08:27 by sgah              #+#    #+#             */
/*   Updated: 2021/11/24 02:53:41 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Webserv.hpp"

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

	if (ac == 2)
	{
		try
		{
			parse.readConf(av[1]);
			parse.parseConf();
		}
		catch(const std::exception& e)
		{
			std::cerr << "Error Parsing: " << e.what() << '\n';
		}
	}
}
