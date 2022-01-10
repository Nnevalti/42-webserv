/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 18:34:08 by sgah              #+#    #+#             */
/*   Updated: 2022/01/10 17:04:03 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Request.hpp"
# include "Cgi.hpp"
# include "autoindex.hpp"

class Response {

	public:

		Response(void);

		Response(Response const & src);
		~Response(void);

		Response &	operator=(const Response &rhs);

		void		resetResponse(ConfigResponse& conf);

		void		createHeader();

		void		parseCgiBody(std::string body);

		void		getMethod(void);
		void		deleteMethod(void);
		void		postMethod(void);

		std::string	readFile(int code);
		std::string readFile(std::string path);

		std::string	findType(std::string contentlocation);

		void		InitResponseProcess(void);
		void		initDirectives(void);
		void		initErrorMap(void);

		std::string	getResponse(void) const;
		int			getCode(void) const;
		bool		getStatus(void) const;

	private:
		static methodMap	initMethods(void);
		static methodMap	_method;

		static stringMap	_typeMap;
		static stringMap	initType(void);

	private:
		ConfigResponse		_config;
		errorMap			status;
		int					_code;
		stringMap			_directives;
		std::string			_header;
		std::string			_body;
		std::string			_response;
		bool				_status;
};
