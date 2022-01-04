/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 12:36:53 by vdescham          #+#    #+#             */
/*   Updated: 2022/01/03 18:03:02 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CgiHandler_H
# define CgiHandler_H

# include "ConfigResponse.hpp"

class CgiHandler
{
	public:
		CgiHandler(void);
		CgiHandler(CgiHandler const & src);
		~CgiHandler(void);

		CgiHandler		&operator=(const CgiHandler &rhs);

		std::string		execute(void);
		char 			**mapToEnv(void);
		void			setEnv(void);
		void			initCgiHandler(ConfigResponse& config);


	private:
		stringMap		_env;
		std::string		_body;
		std::string		_method;
		std::string		_contentSize;
		std::string		_exe;
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
