/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 18:34:08 by sgah              #+#    #+#             */
/*   Updated: 2021/12/08 17:17:48 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_H
# define RESPONSE_H

# include "headers.hpp"
# include "Config.hpp"
# include "ConfigResponse.hpp"
# include "Request.hpp"

class Response {

	public:

		Response(void);

		Response(Response const & src);
		~Response(void);

		Response &	operator=(const Response &rhs);

};


#endif
