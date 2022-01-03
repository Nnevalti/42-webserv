/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 12:36:53 by vdescham          #+#    #+#             */
/*   Updated: 2022/01/03 18:03:02 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_H
# define CGI_H

# include <headers.hpp>
# include <Request.hpp>
# include <Config.hpp>

class CGI
{
	public:
		CGI(void);
		CGI(CGI const & src);
		~CGI(void);

		CGI 		&operator=(const CGI &rhs);

		std::string	execute(void);
		char 		**mapToEnv(void);
		void		initEnv(Request &request, Config &config);


	private:

		stringMap	_env;
		std::string _body;
};


#endif
