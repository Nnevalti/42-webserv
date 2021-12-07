/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 18:34:08 by sgah              #+#    #+#             */
/*   Updated: 2021/12/07 17:55:55 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_H
# define RESPONSE_H

# include "headers.hpp"
# include "Config.hpp"
# include "Request.hpp"

class Response {

	public:

		Response(void);

		Response(Response const & src);
		~Response(void);

		Response &	operator=(const Response &rhs);

		void		initDirectives(void);
		void		setCode(int code);
		void		setErrorMap(StringIntVectorMap pages);
		void		setServer(Config& server);
		void		setRequest(Request& request);
		void		setIndex(stringVector index);
		void		setContentLocation(std::string location);

		void		initResponse(void);
		void		createBody(void);
		void		createHeader(void);
		void		readFile(void);

		void		getMethod(void);

	private:
		static methodMap	initMethods(void);
		static methodMap	_method;

	private:

		std::string					_response;
		int							_code;
		Config						_server;
		Request						_request;
		std::map<int, std::string>	_errorMap;
		stringVector				_index;
		std::string					_contentLocation;
		stringMap					_directives;
};


#endif
