/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 12:36:53 by vdescham          #+#    #+#             */
/*   Updated: 2022/01/03 18:53:41 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI(void) // decide which class we'll use for the cgi conf
{}

CGI::CGI(CGI const & src)
{
	*this = src;
}

CGI::~CGI(void) {}

CGI &	CGI::operator=(const CGI &rhs)
{
	(void)rhs;
	// don't forget this
}

void	CGI::initEnv(Request &request, Config &config)
{
	std::map<std::string, std::string>	headers = request.getHeaders();
	if (headers.find("Auth-Scheme") != headers.end() && headers["Auth-Scheme"] != "")
		_env["AUTH_TYPE"] = headers["Authorization"];

	// madatory env to execute php cgi
	_env["REDIRECT_STATUS"] = "200";

	// Fixed env var
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_SOFTWARE"] = "Webserv/1.0";

	_env["SCRIPT_NAME"] = ; // Path : http:://127.0.0.1/<path>?<query>
	_env["PATH_TRANSLATED"] = "/mnt/nfs/homes/vdescham/Documents/42-webserv/DOC/POC/vdescham/CGI/PHP_TEST/test.php";
	// full path to the ressource /usr/local/etc....
	_env["QUERY_STRING"] = request.getPath();  // Query : http:://127.0.0.1/<path>?<query>

	_env["REQUEST_METHOD"] = request.getMethod();
	_env["CONTENT_LENGTH"] = (static_cast<std::ostringstream*>( &(std::ostringstream() << this->_body.length()) )->str());
	_env["CONTENT_TYPE"] = headers["Content-Type"];
	_env["PATH_INFO"] = request.getPath();
	_env["PATH_TRANSLATED"] = request.getPath();

	// _env["REMOTEaddr"] = inet_ntoa(config.getNetwork().host);
	// _env["REMOTE_IDENT"] = headers["Authorization"];
	// _env["REMOTE_USER"] = headers["Authorization"];


	// this->_env["REQUEST_URI"] = request.getPath() + request.getQuery();

	// if (headers.find("Hostname") != headers.end())
	// 	this->_env["SERVER_NAME"] = headers["Hostname"];
	// else
		_env["SERVER_NAME"] = _env["REMOTEaddr"];

	_env["SERVER_PORT"] = (static_cast<std::ostringstream*>( &(std::ostringstream() << this->_body.length()) )->str());


	_env.insert(request.getEnv().begin(), request.getEnv().end()); // env parameters from client request
}

char **CGI::mapToEnv(void)
{
	char **cgiEnv = new char *[_env.size() + 1];
	std::string tempStr;
	int i = 0;

	for (stringMap::iterator it = _env.begin(); it != _env.end(); it++, i++)
	{
		tempStr = it->first + "=" + it->second;
		cgiEnv[i] = new char[tempStr.size() + 1];
		cgiEnv[i] = std::strcpy(cgiEnv[i], tempStr.c_str());
	}
	return cgiEnv;
}

std::string		CGI::execute(void)
{
	char **cgiEnv;
	pid_t pid;

	int		saveStdin = dup(STDIN_FILENO);
	int		saveStdout = dup(STDOUT_FILENO);
	FILE	*tmpIn = tmpfile();
	FILE	*tmpOut = tmpfile();
	long	fdIn = fileno(tmpIn);
	long	fdOut = fileno(tmpOut);
	std::string newBody;

	cgiEnv = mapToEnv();
	std::string body; // open a file

	write(fdIn, body.c_str(), body.size());
	lseek(fdIn, 0, SEEK_SET);

	if ((pid = fork()) == -1)
		std::cerr << "Error: fork failed, Status: 500 " << '\n';
	else if (pid == 0)
	{
		char * const * nll = NULL;

		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve(_env["SCRIPT_NAME"].c_str(), nll, cgiEnv);
	}
	else
	{
		char	buffer[BUFFER_SIZE + 1] = {0};

		waitpid(0, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);

		int ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, BUFFER_SIZE + 1);
			ret = read(fdOut, buffer, BUFFER_SIZE);
			newBody += buffer;
		}
	}
	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	fclose(tmpIn);
	fclose(tmpOut);
	close(fdIn);
	close(fdOut);
	close(saveStdin);
	close(saveStdout);

	for (int i = 0;  cgiEnv[i]; i++)
		delete cgiEnv[i];
	delete [] cgiEnv;
	if (pid == 0)
		exit(0);
	return newBody;
}
