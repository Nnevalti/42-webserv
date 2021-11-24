/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 16:16:28 by sgah              #+#    #+#             */
/*   Updated: 2021/11/24 05:59:04 by sgah             ###   ########.fr       */
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

		friend std::ostream	&operator<<(std::ostream &out, const Config &server);
		Config &	operator=(const Config &rhs);

		void			addNetwork(t_network network);
		void			addServerName(stringVector serverName);
		void			addRoot(std::string root);
		void			addClientBodyBufferSize(int buffer);
		void			addCgiParam(stringMap cgiParam);
		void			addCgiPass(std::string cgiPass);
		void			addLocation(serverMap locations);
		void			addAllowedMethods(stringVector methods);
		void			addIndex(stringVector index);
		void			addAutoIndex(bool autoIndex);
		void			addAlias(std::string alias);
		void			addErrorPage(stringMap pages);

		netVector		getNetwork(void);
		stringVector	getServerName(void);
		std::string		getRoot(void);
		int				getClientBodyBufferSize(void);
		stringMap		getCgiParam(void);
		std::string		getCgiPass(void);
		serverMap		getLocation(void);
		stringVector	getAllowedMethods(void);
		stringVector	getIndex(void);
		bool			getAutoIndex(void);
		std::string		getAlias(void);
		stringMap		getErrorPage(void);

	private:
		netVector		_network;
		std::string		_root;
		stringVector	_server_name;
		stringMap		_error_page;
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
