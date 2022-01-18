/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 12:36:53 by vdescham          #+#    #+#             */
/*   Updated: 2022/01/04 20:56:21 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

Cgi::Cgi(void)
{
	_body = "";
	_method = "";
	_contentSize = "";
	_contentType = "";
	_contentPath = "";
	_contentFile = "";
	_contentPathFile = "";
	_toExe = "";
	_query = "";
	_port = "";
	_host = "";
	_authorisation = "";
	_protocol = "";
	_serverName = "";
}

Cgi::Cgi(Cgi const & src)
{
	*this = src;
}

Cgi::~Cgi(void) {}

Cgi &				Cgi::operator=(const Cgi &rhs)
{
	_body = rhs._body;
	_method = rhs._method;
	_contentSize = rhs._contentSize;
	_contentType = rhs._contentType;
	_contentPath = rhs._contentPath;
	_contentFile = rhs._contentFile;
	_contentPathFile = rhs._contentPathFile;
	_toExe = rhs._toExe;
	_query = rhs._query;
	_port = rhs._port;
	_host = rhs._host;
	_authorisation = rhs._authorisation;
	_protocol = rhs._protocol;
	_serverName = rhs._serverName;
	return (*this);
}

static std::string	ft_itoa(int nb)
{
	return (static_cast<std::ostringstream*>( &(std::ostringstream() << nb) )->str());
}

void				Cgi::initCgiData(ConfigResponse& Config)
{
	stringMap	headers(Config.getRequest().getHeaders());

	_body = Config.getRequest().getBody();
	_cgiPass = Config.getLocation().getCgiPass();
	_method = Config.getRequest().getMethod();
	_contentSize = ft_itoa(Config.getRequest().bodySize);
	_contentType = headers["Content-Type"];
	_contentFile = Config.getLocationPath();
	_contentPath = Config.getLocationFile();
	_toExe = Config.getContent();
	_query = Config.getRequest().getQuery();
	_contentPathFile = Config.getContentLocation();
	_port = ft_itoa(Config.getServer().getNetwork().port);
	_host = std::string(inet_ntoa(Config.getServer().getNetwork().host));
	if (headers.find("Auth-Scheme") != headers.end() && headers["Auth-Scheme"] != "")
		_authorisation = headers["Authorization"];
	_protocol = "HTTP/1.1";
	_serverName = "Webserv";
	_interface	= "CGI/1.1";
	_status = "200";
}

void				Cgi::setEnv(void)
{
	_env["AUTH_TYPE"] = _authorisation;
	_env["REDIRECT_STATUS"] = _status;
	_env["GATEWAY_INTERFACE"] = _interface;
	_env["SCRIPT_NAME"] = _toExe;
	_env["SCRIPT_FILENAME"] = _toExe;
	_env["REQUEST_METHOD"] = _method;
	_env["CONTENT_LENGTH"] = _contentSize;
	_env["CONTENT_TYPE"] = _contentType;
	_env["PATH_INFO"] = _contentPath;
	_env["PATH_TRANSLATED"] = _contentPath;
	_env["QUERY_STRING"] = _query;
	_env["REMOTEaddr"] = _host;
	_env["REMOTE_IDENT"] = _authorisation;
	_env["REMOTE_USER"] = _authorisation;
	_env["REQUEST_URI"] = _contentPath;
	_env["SERVER_NAME"] = _host;
	_env["SERVER_PORT"] = _port;
	_env["SERVER_PROTOCOL"] = _protocol;
	_env["SERVER_SOFTWARE"] = _serverName;
}

char				**Cgi::mapToTab(void)
{
	char **CgiEnv = new char *[_env.size() + 1];
	std::string tempStr;
	int i = 0;

	for (stringMap::iterator it = _env.begin(); it != _env.end(); it++, i++)
	{
		tempStr = it->first + "=" + it->second;
		CgiEnv[i] = new char[tempStr.size() + 1];
		CgiEnv[i] = std::strcpy(CgiEnv[i], tempStr.c_str());
	}
	CgiEnv[i] = NULL;
	return CgiEnv;
}

std::string			Cgi::execute(void)
{
	char		**CgiEnv = mapToTab();
	int			saveStdin = dup(STDIN_FILENO);
	int			saveStdout = dup(STDOUT_FILENO);
	FILE		*tmpIn = tmpfile();
	FILE		*tmpOut = tmpfile();
	int			In = fileno(tmpIn);
	int			Out = fileno(tmpOut);
	std::string	body;
	pid_t		pid;

	if (write(In, _body.c_str(), std::atoi(_contentSize.c_str())) == -1)
	{
		for (int i = 0;  CgiEnv[i]; i++)
			delete CgiEnv[i];
		delete [] CgiEnv;
		return ("Status: 500\r\n");
	}
	lseek(In, 0, SEEK_SET);

	if ((pid = fork()) == -1)
	{
		for (int i = 0;  CgiEnv[i]; i++)
			delete CgiEnv[i];
		delete [] CgiEnv;
		return ("Status: 500\r\n");
	}
	else if (pid == 0)
	{
		char * const * nll = NULL;

		dup2(In, STDIN_FILENO);
		dup2(Out, STDOUT_FILENO);
		execve(_cgiPass.c_str(), nll, CgiEnv);
		if (write(STDOUT_FILENO, "Status: 500\r\n", 15) == -1)
		{
			for (int i = 0;  CgiEnv[i]; i++)
				delete CgiEnv[i];
			delete [] CgiEnv;
			return ("Status: 500\r\n");
		}
	}
	else
	{
		char	buffer[BUFFER_SIZE + 1];

		waitpid(0, NULL, 0);
		lseek(Out, 0, SEEK_SET);

		int ret = 1;
		while (ret > 0)
		{
			ret = read(Out, buffer, BUFFER_SIZE);
			if (ret != -1)
				buffer[ret] = '\0';
			body += buffer;
		}
	}
	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	fclose(tmpIn);
	fclose(tmpOut);
	close(In);
	close(Out);
	close(saveStdin);
	close(saveStdout);

	if (pid == 0)
		exit(0);
	for (int i = 0;  CgiEnv[i]; i++)
		delete CgiEnv[i];
	delete [] CgiEnv;
	return body;
}
