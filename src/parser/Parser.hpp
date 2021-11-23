/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 15:32:31 by sgah              #+#    #+#             */
/*   Updated: 2021/11/23 23:11:26 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "headers.hpp"
# include "Config.hpp"23

class Parser {

	public:

		Parser(void);

		Parser(Parser const & src);
		~Parser(void);

		Parser&				operator=(const Parser &rhs);

		void				readconf(const char *file);
		void				parseconf(void);

	private:
		stringVector		_configfile;
		std::vector<Config>	_ConfigServers;
};


#endif
