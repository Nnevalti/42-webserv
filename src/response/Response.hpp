/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 18:34:08 by sgah              #+#    #+#             */
/*   Updated: 2021/12/06 21:15:27 by sgah             ###   ########.fr       */
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


	private:

		std::string					_response;
		int							_code;
		Config						_server;
		Request						_request;
		std::map<int, std::string>	_errorMap;
		stringMap					_directives;
};


#endif
