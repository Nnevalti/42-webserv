/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 19:51:16 by sgah              #+#    #+#             */
/*   Updated: 2022/01/12 15:46:55 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "headers.hpp"

class Request {

	public:

		Request(void);

		Request(Request const & src);
		~Request(void);

		Request &	operator=(const Request &rhs);
		friend std::ostream	&operator<<(std::ostream &out, const Request &request);

		void					resetDirective(void);
		void					resetRequest(void);

		std::string				getPath(void) const;
		std::string				getMethod(void) const;
		std::string				getVersion(void) const;
		std::string				getBody(void) const;
		stringMap				getHeaders(void) const;
		std::string				getHeader(std::string str);
		const stringMap&		getEnv(void) const;
		int						getCode(void) const;
		int						getPort(void) const;
		t_network				getNetwork(void) const;
		std::string				getQuery(void) const;

		void					setMethod(std::string method);
		void					setPath(std::string path);
		void					setHeader(std::string token, std::string value);
		void					setVersion(std::string version);
		void					setBody(std::string body);
		void					setHeaders(stringMap headers);
		void					setEnv(stringMap env);
		void					setRet(int ret);
		void					setNetwork(std::string IpPort);
		void					setEnvForCgi(std::string token, std::string value);

		std::string				raw_request;
		stringVector			header;

		bool					header_ready;
		bool					body_ready;
		int						contentSize;
		unsigned long			bodySize;

	private:

		std::string				_method;
		std::string				_version;
		int						_ret;
		std::string				_body;
		int						_port;
		std::string				_path;
		std::string				_query;
		stringMap				_headers;
		stringMap				_env_for_cgi;
		t_network				_network;
};
