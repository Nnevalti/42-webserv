/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 15:08:27 by sgah              #+#    #+#             */
/*   Updated: 2021/11/22 16:28:58 by sgah             ###   ########.fr       */
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
		parse.config(av[1]);
	}
}
