/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 19:51:16 by sgah              #+#    #+#             */
/*   Updated: 2021/11/30 21:22:48 by sgah             ###   ########.fr       */
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

		std::string											getPath(void) const;
		std::string											getMethod(void) const;
		std::string											getVersion(void) const;
		std::string											getBody(void) const;
		stringMap											getHeaders(void) const;
		std::string											getHeader(std::string str);
		const stringMap &									getEnv(void) const;
		const std::list<std::pair<std::string, float> > &	getLang(void) const;
		int													getRet(void) const;
		const std::string &									getQuery(void) const;
		int													getStatus(void) const;

		void												setMethod(std::string method);
		void												setPath(std::string path);
		void												setVersion(std::string version);
		void												swetBody(std::string body);
		void												setHeaders(stringMap headers);
		void												setEnv(stringMap env);
		void												setLang(std::list<std::pair<std::string, float> > lang);
		void												setRet(int ret);
		void												setQuery(std::string query);
		void												setStatus(int state);

	private:

		std::string									_method;
		std::string									_version;
		int											_ret;
		std::string									_body;
		int											_port;
		std::string									_path;
		std::string									_query;
		int											_status;
		stringMap									_headers;
		stringMap									_env_for_cgi;
		std::list<std::pair<std::string, float> >	_lang;
};


#endif
