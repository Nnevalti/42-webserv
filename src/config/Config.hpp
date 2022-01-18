/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 16:16:28 by sgah              #+#    #+#             */
/*   Updated: 2022/01/18 03:34:25 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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
		void				setCgiPass(std::string cgiPass);
		void				setLocation(std::string location_name, Config location);
		void				setAllowedMethods(stringVector methods);
		void				setIndex(std::string index);
		void				setAutoIndex(bool autoIndex);
		void				setAlias(std::string alias);
		void				setErrorPage(std::string page, std::vector<int> codes);
		void				setErrorCode(std::string page, int code);
		void				setReturn(std::string code, std::string url);

		t_network&			getNetwork(void);
		stringVector&		getServerName(void);
		std::string&		getRoot(void);
		size_t&				getClientBodyBufferSize(void);
		std::string&		getCgiPass(void);
		serverMap&			getLocation(void);
		stringVector&		getAllowedMethods(void);
		std::string&		getIndex(void);
		bool&				getAutoIndex(void);
		std::string&		getAlias(void);
		stringIntVectorMap&	getErrorPage(void);
		pairString&			getReturn(void);
		bool				extension;

	private:
		t_network			_network;
		std::string			_root;
		stringVector		_server_name;
		stringIntVectorMap	_error_page;
		size_t				_client_body_buffer_size;
		std::string			_cgi_pass;
		serverMap			_location;
		stringVector		_allowed_methods;
		std::string			_index;
		bool				_autoindex;
		std::string			_alias;
		pairString			_return;
};
