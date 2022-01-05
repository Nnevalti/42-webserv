/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 12:36:53 by vdescham          #+#    #+#             */
/*   Updated: 2022/01/05 01:41:00 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Cgi_H
# define Cgi_H

# include "ConfigResponse.hpp"

class Cgi
{
	public:
		Cgi(void);
		Cgi(Cgi const & src);
		~Cgi(void);

		Cgi		&operator=(const Cgi &rhs);

		std::string		execute(void);
		char 			**mapToTab(void);
		void			setEnv(void);
		void			initCgiData(ConfigResponse& config);


	private:
		stringMap		_env;
		std::string		_body;
		std::string		_cgiPass;
		std::string		_method;
		std::string		_contentSize;
		std::string		_toExe;
		std::string		_contentType;
		std::string		_contentPath;
		std::string		_contentFile;
		std::string		_contentPathFile;
		std::string		_query;
		std::string		_port;
		std::string		_host;
		std::string		_authorisation;
		std::string		_protocol;
		std::string		_serverName;
		std::string		_interface;
		std::string		_status;
};


#endif
