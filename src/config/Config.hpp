/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 16:16:28 by sgah              #+#    #+#             */
/*   Updated: 2021/12/07 19:21:14 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "headers.hpp"

class Config {

	public:

		Config(void);

		Config(Config const & src);
		~Config(void);

		friend std::ostream	&operator<<(std::ostream &out, const Config &server);
		Config &	operator=(const Config &rhs);

		void				setNetwork(t_network &network);
		void				setServerName(stringVector serverName);
		void				setRoot(std::string root);
		void				setClientBodyBufferSize(int buffer);
		void				setCgiParam(stringVector cgiParam);
		void				setCgiPass(std::string cgiPass);
		void				setLocation(std::string location_name, Config location);
		void				setAllowedMethods(stringVector methods);
		void				setIndex(stringVector index);
		void				setAutoIndex(bool autoIndex);
		void				setAlias(std::string alias);
		void				setErrorPage(std::string page, std::vector<int> codes);

		t_network			getNetwork(void);
		stringVector		getServerName(void);
		std::string			getRoot(void);
		size_t				getClientBodyBufferSize(void);
		stringVector		getCgiParam(void);
		std::string			getCgiPass(void);
		serverMap			getLocation(void);
		stringVector		getAllowedMethods(void);
		stringVector		getIndex(void);
		bool				getAutoIndex(void);
		std::string			getAlias(void);
		bool				getisAliasSet(void);
		StringIntVectorMap	getErrorPage(void);

	private:
		t_network			_network;
		std::string			_root;
		stringVector		_server_name;
		StringIntVectorMap	_error_page;
		size_t				_client_body_buffer_size;
		stringVector		_cgi_param;
		std::string			_cgi_pass;
		serverMap			_location;
		stringVector		_allowed_methods;
		stringVector		_index;
		bool				_autoindex;
		std::string			_alias;
		bool				_alias_set;
};


#endif
