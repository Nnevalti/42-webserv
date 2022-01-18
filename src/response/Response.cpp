/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 18:34:09 by sgah              #+#    #+#             */
/*   Updated: 2022/01/18 03:10:13 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

static std::string	ft_itoa(int nb)
{
	return (static_cast<std::ostringstream*>( &(std::ostringstream() << nb) )->str());
}

static int		checkReadPermission(const std::string &path)
{
	struct stat s;

	if (stat(path.c_str(), &s) == 0)
		if(s.st_mode & S_IROTH)
			return (1);
	return (0);
}

static int		checkWritePermission(const std::string &path)
{
	struct stat s;

	if (stat(path.c_str(), &s) == 0)
		if(s.st_mode & S_IWOTH)
			return (1);
	return (0);
}

static int		checkPath(const std::string &path)
{
	struct stat s;

	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
			return IS_A_DIRECTORY;
		else if (s.st_mode & S_IFREG)
			return IS_A_FILE;
		else
			return IS_SOMETHING_ELSE;
	}
	else
		return IS_SOMETHING_ELSE;
}

static std::string	getDate(void)
{
	char			buffer[100];
	struct timeval	tv;
	struct tm		*tm;

	gettimeofday(&tv, NULL);
	tm = gmtime(&tv.tv_sec);
	strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
	return (std::string(buffer));
}

static std::string	getLastModif(const std::string& path)
{
	char			buffer[100];
	struct stat		stats;
	struct tm		*tm;

	if (stat(path.c_str(), &stats) == 0)
	{
		tm = gmtime(&stats.st_mtime);
		strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
		return (std::string(buffer));
	}
	return ("");
}

Response::Response(void) {}

Response::Response(Response const & src)
{
	*this = src;
}

Response::~Response(void) {}

Response &	Response::operator=(const Response &rhs)
{
	_config = rhs._config;
	return (*this);
}

std::string	Response::getResponse(void) const
{
	return (_response);
}

int			Response::getCode(void) const
{
	return _code;
}

methodMap	Response::initMethods(void)
{
	methodMap map;

	map["GET"] = &Response::getMethod;
	map["DELETE"] = &Response::deleteMethod;
	map["POST"] = &Response::postMethod;

	return map;
}

methodMap Response::_method = Response::initMethods();

stringMap	Response::initType()
{
	stringMap tmp;

	tmp["acc"] = "audio/aac";
	tmp["abw"] = "application/x-abiword";
	tmp["avi"] = "video/x-msvideo";
	tmp["bin"] = "application/octet-stream";
	tmp["bmp"] = "image/bmp";
	tmp["bz"] = "application/x-bzip";
	tmp["bz2"] = "application/x-bzip2";
	tmp["csh"] = "application/x-csh";
	tmp["css"] = "text/css";
	tmp["csv"] = "text/csv";
	tmp["doc"] = "application/msword";
	tmp["gif"] = "image/gif";
	tmp["htm"] = "text/html";
	tmp["html"] = "text/html";
	tmp["ico"] = "image/x-icon";
	tmp["ics"] = "text/calendar";
	tmp["jar"] = "application/java-archive";
	tmp["jpeg"] = "image/jpeg";
	tmp["jpg"] = "image/jpeg";
	tmp["js"] = "application/javascript";
	tmp["json"] = "application/json";
	tmp["mid"] = "audio/midi";
	tmp["midi"] = "audio/midi";
	tmp["mpeg"] = "video/mpeg";
	tmp["oga"] = "audio/ogg";
	tmp["ogv"] = "video/ogg";
	tmp["ogx"] = "application/ogg";
	tmp["otf"] = "font/otf";
	tmp["png"] = "image/png";
	tmp["pdf"] = "application/pdf";
	tmp["rar"] = "application/x-rar-compressed";
	tmp["rtf"] = "application/rtf";
	tmp["sh"] = "application/x-sh";
	tmp["svg"] = "image/svg+xml";
	tmp["tif"] = "image/tiff";
	tmp["tiff"] = "image/tiff";
	tmp["ts"] = "application/typescript";
	tmp["ttf"] = "font/ttf";
	tmp["vsd"] = "application/vnd.visio";
	tmp["wav"] = "audio/x-wav";
	tmp["weba"] = "audio/webm";
	tmp["webm"] = "video/webm";
	tmp["webp"] = "image/webm";
	tmp["woff"] = "font/woff";
	tmp["woff2"] = "font/woff2";
	tmp["xhtml"] = "application/xhtml+xml";
	tmp["xml"] = "application/xml";
	tmp["zip"] = "application/zip";

	return (tmp);
}

stringMap Response::_typeMap = Response::initType();

std::string	Response::findType(std::string contentlocation)
{
	std::string type(contentlocation.substr(contentlocation.rfind(".") + 1, contentlocation.size() - contentlocation.rfind(".")));
	if (_typeMap.find(type) == _typeMap.end())
		return "text/plain";
	return _typeMap[type];
}

void	Response::initDirectives(void)
{
	_directives["Allow"] = "";
	_directives["Content-Language"] = "";
	_directives["Content-Length"] = "";
	_directives["Content-Location"] = "";
	_directives["Content-Type"] = "";
	_directives["Date"] = "";
	_directives["Last-Modified"] = "";
	_directives["Connection"] = "";
	_directives["Location"] = "";
	_directives["Retry-After"] = "";
	_directives["Server"] = "Webserv";
	_directives["Transfer-Encoding"] = "identity";
	_directives["WwwAuthenticate"] = "";
	_directives["Set-Cookie"] = "";
}

void			Response::initErrorMap(void)
{
	status[100] = "Continue";
	status[200] = "OK";
	status[201] = "Created";
	status[204] = "No Content";
	status[301] = "Moved Permanently";
	status[308] = "Permanent Redirect";
	status[400] = "Bad Request";
	status[403] = "Forbidden";
	status[404] = "Not Found";
	status[405] = "Method Not Allowed";
	status[408] = "Request Timeout";
	status[413] = "Payload Too Large";
	status[500] = "Internal Server Error";
}

void		Response::resetResponse(ConfigResponse& conf)
{
	_code = conf.getCode();
	_header = "";
	_body = "";
	initErrorMap();
	initDirectives();
	_config = conf;
}

std::string			Response::readFile(int code)
{
	std::string	path(_config.getErrorPath(code));
	std::ofstream		file;
	std::stringstream	buffer;

	if (checkPath(path) == IS_A_FILE)
	{
		file.open(path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
		{
			_code = 404;
			_directives["Content-Type"] = "text/html";
			_body = "<!DOCTYPE html>\n<html>\n\t<title>404 Not Found</title>\n\t<body>\n\t\t<div>\n\t\t\t<H1>404 Not Found</H1>\n\t\t</div>\n\t</body>\t</html>";
			return (ft_itoa(_body.size()));
		}
		buffer << file.rdbuf();
		file.close();
		_directives["Content-Type"] = "text/html";
		_body = buffer.str();
		return (ft_itoa(_body.size()));
	}
	else
	{
		_code = 404;
		_directives["Content-Type"] = "text/html";
		_body = "<!DOCTYPE html>\n<html>\n\t<title>404 Not Found</title>\n\t<body>\n\t\t<div>\n\t\t\t<H1>404 Not Found</H1>\n\t\t</div>\n\t</body>\t</html>";
		return (ft_itoa(_body.size()));
	}
	return (ft_itoa(_body.size()));
}

std::string			Response::readFile(std::string path)
{
	std::ofstream		file;
	std::stringstream	buffer;

	if (checkPath(_config.getContentLocation()) == IS_A_DIRECTORY && _config.getAutoIndex())
		_body = createAutoindexPage(_config.getContentLocation(), _config.getRequest().getPath());
	else if (checkPath(path) == IS_A_FILE)
	{

		file.open(path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
		{
			_code = 404;
			return (readFile(_code));
		}
		buffer << file.rdbuf();
		file.close();
		_directives["Content-Type"] = findType(path);
		if (checkReadPermission(path) == 0)
		{
			_code = 403;
			return (readFile(_code));
		}
		_body = buffer.str();
	}
	else
	{
		_code = 404;
		readFile(_code);
	}
	return (ft_itoa(_body.size()));
}

static std::string gen_random(const int len)
{
	static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string tmp_s;
	tmp_s.reserve(len);

	for (int i = 0; i < len; ++i)
		tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];

	return tmp_s;
}

void		Response::createHeader(void)
{
	stringVector tmp(_config.getAllow());

	if(_code == 405)
	{
		for (stringVector::iterator i = tmp.begin(); i != tmp.end(); i++)
			if (i + 1 == tmp.end())
				_directives["Allow"] += *i;
			else
				_directives["Allow"] += *i + " ";
	}
	_directives["Content-Language"] = _config.getLanguage();
	if (_code != 404)
		_directives["Content-Location"] = _config.getContentLocation();
	_directives["Date"] = getDate();
	_directives["Last-Modified"] = getLastModif(_config.getContentLocation());
	if (_code == 503 || _code == 429 || _code == 301)
		_directives["Retry-After"] = "2";
	if (_code == 401)
		_directives["WWW-Authenticate"] = "Basic realm=\"Access requires authentification\" charset=\"UTF-8\"";
	_header += "HTTP/1.1 " + ft_itoa(_code);
	_header += " " + status[_code] + "\r\n";
	for (stringMap::const_iterator i = _directives.begin(); i != _directives.end(); i++)
		if (i->second != "")
			_header+= i->first + ": " + i->second + "\r\n";
	_response = _header + "\r\n" + _body;
}

void		Response::InitResponseProcess(std::string id)
{
	stringSet tmp(_config.getAllowMethod());

	if (_config.getCookie("user_id") == "" && _config.getCookie("cookieconsent_status") == "allow")
	{
		std::cout << _config.getCookie("user_id") << std::endl;
		if (id == "")
			userId = gen_random(32);
		else
			userId = id;
		_directives["Set-Cookie"] = "user_id=" + userId;
	}

	if (_config.getLocation().getReturn().first != "")
	{
		if(_config.getRequest().getMethod() != "POST")
			_code = std::atoi(_config.getLocation().getReturn().first.c_str());
		else
			_code = 308;
		_directives["Location"] = _config.getLocation().getReturn().second;
		createHeader();
		return ;
	}

	if (tmp.find(_config.getRequest().getMethod()) == tmp.end())
		_code = 405;
	else if (_config.getClientBodyBufferSize() < _config.getRequest().bodySize)
		_code = 413;

	if(_code == 408)
	{
		_directives["Connection"] = "close";
		createHeader();
	}
	else if (_code != 200)
	{
		_directives["Content-Length"] = readFile(_code);
		createHeader();
	}
	else if(_method.find(_config.getRequest().getMethod()) != _method.end())
		(this->*Response::_method[_config.getRequest().getMethod()])();
}

void		Response::parseCgiBody(std::string body)
{
	size_t	start;
	size_t	startBody = 0;
	size_t sepSize = 0;

	if((start = body.find("Status: ")) != std::string::npos)
	{
		_code = std::atoi(body.substr(start + 8, 3).c_str());
		_directives["Content-Length"] = readFile(_code);
	}
	else if ((start = body.find("Content-type: ")) != std::string::npos)
	{
		if ((startBody = body.find("\r\n\r\n", start)) != std::string::npos)
			sepSize = 4;
		else if((startBody = body.find("\n\n", start)) != std::string::npos)
			sepSize = 2;

		std::string line = body.substr(start, startBody - start);

		_body = body.substr(startBody + sepSize);
		_directives["Content-Type"] = line.substr(14);
		_directives["Content-Length"] = ft_itoa(_body.size());;
	}
}

void		Response::getMethod(void)
{
	if(checkPath(_config.getContentLocation()) == IS_A_DIRECTORY &&
	_config.getContentLocation() == (_config.getLocation().getRoot() + _config.getLocation().getAlias()))
	{
		_config.setContent(_config.getContentLocation() + "/" + _config.getIndex());
		if (_config.getIndex() != "")
			_config.setAutoIndex(false);
	}
	else
		_config.setContent(_config.getContentLocation());

	if (!_config.getCgiPass().empty())
	{
		Cgi cgi;
		std::string tmpBody;
		cgi.initCgiData(_config);
		cgi.setEnv();
		tmpBody = cgi.execute();
		parseCgiBody(tmpBody);
	}
	else
		_directives["Content-Length"] = readFile(_config.getContent());
	createHeader();
}

void		Response::deleteMethod(void)
{
	std::string path(_config.getContentLocation());

	if (checkPath(path) != IS_SOMETHING_ELSE)
	{
		if (checkWritePermission(path) && remove(path.c_str()) == 0)
			_code = 204;
		else
			_code = 403;
	}
	else
		_code = 404;
	if (_code != 204)
		_directives["Content-Length"] = readFile(_code);
	createHeader();
}

void		Response::postMethod(void)
{
	if(checkPath(_config.getContentLocation()) == IS_A_DIRECTORY &&
	_config.getContentLocation() == (_config.getLocation().getRoot() + _config.getLocation().getAlias()))
		_config.setContent(_config.getContentLocation() + "/" + _config.getIndex());
	else
		_config.setContent(_config.getContentLocation());
	if (!_config.getCgiPass().empty())
	{
		Cgi cgi;
		std::string tmpBody;
		cgi.initCgiData(_config);
		cgi.setEnv();
		tmpBody = cgi.execute();
		parseCgiBody(tmpBody);
	}
	else
		_code = 204;
	if (_code == 200)
	{
		std::string host = std::string(inet_ntoa(_config.getServer().getNetwork().host));
		std::string port = ft_itoa(_config.getServer().getNetwork().port);
		std::string location = host + ":" + port + "/upload";
		_code = 201;
		_directives["Location"] = location;
	}

	createHeader();
}
