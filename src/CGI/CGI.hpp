/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdescham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 12:36:53 by vdescham          #+#    #+#             */
/*   Updated: 2021/12/07 12:36:53 by vdescham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_H
# define CGI_H

# include <iostream>

class CGI
{
	public:
		CGI(CGI const & src);
		~CGI(void);

		CGI 	&operator=(const CGI &rhs);

		std::string execute(void)

	private:
		void initEnv(Request &request, Config &config);

		stringMap	_env;
		std::string _body;
};


#endif
