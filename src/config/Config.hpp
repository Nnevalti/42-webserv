/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 16:16:28 by sgah              #+#    #+#             */
/*   Updated: 2021/11/23 23:51:13 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

# include "headers.hpp"

class Config {

	public:

		Config(void);

		Config(Config const & src);
		~Config(void);

		Config &	operator=(const Config &rhs);

		void			addNetwork(stringVector args);
		void			addServerNAme(stringVector args);
		void			addRoot(stringVector args);
		void			addClientBodyBufferSize(stringVector args);
		void			addCgiParam(stringVector args);
		void			addCgiPass(stringVector args);
		void			addLocation(serverMap locations);
		void			addAllowedMethods(stringVector args);
		void			addIndex(stringVector args);
		void			addAutoIndex(stringVector args);
		void			addAlias(stringVector args);

		netVector		getNetwork(void);
		stringVector	getServerNAme(void);
		std::string		getRoot(void);
		int				getClientBodyBufferSize(void);
		stringMap		getCgiParam(void);
		std::string		getCgiPass(void);
		serverMap		getLocation(void);
		stringVector	getAllowedMethods(void);
		stringVector	getIndex(void);
		bool			getAutoIndex(void);
		std::string		getAlias(void);

	private:
		netVector		_network;
		std::string		_root;
		stringVector	_server_name;
		int				_client_body_buffer_size;
		stringMap		_cgi_param;
		std::string		_cgi_pass;
		serverMap		_location;
		stringVector	_allowed_methods;
		stringVector	_index;
		bool			_autoindex;
		std::string		_alias;
		bool			_alias_set;

};


#endif
