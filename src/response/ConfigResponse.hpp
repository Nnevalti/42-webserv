/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigResponse.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:28:48 by sgah              #+#    #+#             */
/*   Updated: 2021/12/09 01:37:39 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGRESPONSE_H
# define CONFIGRESPONSE_H

# include "headers.hpp"
# include "Config.hpp"

class ConfigResponse {

	public:

		ConfigResponse(void);

		ConfigResponse(ConfigResponse const & src);
		~ConfigResponse(void);

		ConfigResponse &	operator=(const ConfigResponse &rhs);

		void				setServer(Config server);
		void				setLocation(Config location);
		void				setLocationPath(std::string path);
		void				setLocationFile(std::string path);
		void				setErrorMap(stringIntVectorMap map);
		void				setClientBodyBufferSize(size_t size);
		void				setCgiParam(stringVector cgiParam);
		void				setCgiPass(std::string cgiPass);
		void				setAllowMethod(stringVector allow);
		void				setLanguage(std::string lang);
		void				setAutoIndex(bool Index);
		void				setIndex(stringVector index);
		void				setContentLocation(std::string content);

		Config				getServer(void) const;
		Config				getLocation(void) const;
		std::string			getLocationPath(void) const;
		std::string			getLocationFile(void) const;
		errorMap			getErrorMap(void) const;
		size_t				getClientBodyBufferSize(void) const;
		stringVector		getCgiParam(void) const;
		std::string			getCgiPass(void) const;
		stringSet			getAllowMethod(void) const;
		std::string			getLanguage(void) const;
		bool				getAutoIndex(void) const;
		stringVector		getIndex(void) const;
		std::string			getContentLocation(void) const;

	private:
		Config			_server;
		Config			_location;
		std::string		_locationPath;
		std::string		_locationFile;
		errorMap		_errorPage;
		size_t			_clientBodyBufferSize;
		stringVector	_cgiParam;
		std::string		_cgiPass;
		stringSet		_allowMethod;
		std::string		_acceptLanguage;
		bool			_autoIndex;
		stringVector	_index;
		std::string		_contentLocation;
};


#endif
