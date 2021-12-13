/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 19:51:16 by sgah              #+#    #+#             */
/*   Updated: 2021/12/11 14:39:22 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_H
# define REQUEST_H

# include "headers.hpp"

class Request {

	public:

		Request(void);

		Request(Request const & src);
		~Request(void);

		Request &	operator=(const Request &rhs);
		friend std::ostream	&operator<<(std::ostream &out, const Request &request);

		void					resetDirective(void);

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

	private:

		std::string				_method;
		std::string				_version;
		int						_ret;
		std::string				_body;
		int						_port;
		std::string				_path;
		stringMap				_headers;
		stringMap				_env_for_cgi;
		t_network				_network;
};


#endif
