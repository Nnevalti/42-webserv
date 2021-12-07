/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseResponse.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 19:44:34 by sgah              #+#    #+#             */
/*   Updated: 2021/12/07 02:09:36 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

void					Parser::parseResponse(Request& request, Response& response, Config& server)
{
	response.initDirectives();
	response.setErrorMap(server.getErrorPage());
	response.setServer(server);
	if (std::find(server.getAllowedMethods().begin(), server.getAllowedMethods().end(), request.getMethod()) == server.getAllowedMethods().end())
		response.setCode(405);
	else if (server.getClientBodyBufferSize() < request.getBody().size())
		response.setCode(413);
	response.setCode(200);
}
