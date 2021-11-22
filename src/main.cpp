/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 15:08:27 by sgah              #+#    #+#             */
/*   Updated: 2021/11/22 18:22:11 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

/**========================================================================
 **                           MAIN
 *?   This is the starting point of webserv
 *@param ac int  number of parameters
 *@param av char**  tab of parameters
 *@return int
 *========================================================================**/
int		main(int ac, char **av)
{
	Parser parse;

	if (ac == 2)
	{
		try
		{
			parse.readconf(av[1]);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
}
