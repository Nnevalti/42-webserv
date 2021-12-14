#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void initCgiEnv(std::map<std::string, std::string> &mapCgiEnv)
{
	// Mandatory Env Var
	mapCgiEnv["REQUEST_METHOD"] = "GET";
	mapCgiEnv["CONTENT_TYPE"] = "text/php";
	mapCgiEnv["PATH_INFO"] = "test.php";
	mapCgiEnv["PATH_TRANSLATED"] = "/mnt/nfs/homes/vdescham/Documents/42-webserv/DOC/POC/vdescham/CGI/PHP_TEST/test.php";

	// Specification Env Var
	mapCgiEnv["SERVER_PROTOCOL"] = "HTTP/1.1";
	mapCgiEnv["SERVER_NAME"] = "127.0.0.1";
	mapCgiEnv["SERVER_PORT"] = "8080";
	mapCgiEnv["SERVER_SOFTWARE"] = "Webserv/1.1";
	mapCgiEnv["GATEWAY_INTERFACE"] = "CGI/1.1";
	mapCgiEnv["SCRIPT_NAME"] = "/mnt/nfs/homes/vdescham/Documents/42-webserv/cgi/linux_phpcgi";

	// PHP Mandatory
	mapCgiEnv["REDIRECT_STATUS"] = "200";
}

char **mapToEnv(std::map<std::string, std::string> mapCgiEnv)
{
	char **cgiEnv = new char *[mapCgiEnv.size() + 1];
	std::string tempStr;
	int i = 0;

	for (std::map<std::string, std::string>::iterator it = mapCgiEnv.begin(); it != mapCgiEnv.end(); it++, i++)
	{
		tempStr = it->first + "=" + it->second;
		cgiEnv[i] = new char[tempStr.size() + 1];
		cgiEnv[i] = strcpy(cgiEnv[i], tempStr.c_str());
	}
	return cgiEnv;
}

int main(void)
{
	std::string cgiPath = "/mnt/nfs/homes/vdescham/Documents/42-webserv/cgi/linux_phpcgi";
	std::map<std::string, std::string> mapCgiEnv;
	char **cgiEnv;
	pid_t pid;

	int		saveStdin = dup(STDIN_FILENO);
	int		saveStdout = dup(STDOUT_FILENO);
	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);
	std::string newBody;

	initCgiEnv(mapCgiEnv);
	cgiEnv = mapToEnv(mapCgiEnv);
	std::string body = "<HTML> \
			<HEAD> \
				<TITLE>About this Server</TITLE> \
			</HEAD> \
			<BODY> \
				<H1>About this Server</H1> \
			<HR><PRE> \
			Server Name:      <?php echo $_ENV[\"SERVER_NAME\"] ?> <br> \
			Running on Port:  <?php echo $_ENV[\"SERVER_PORT\"] ?> <br> \
			Server Software:  <?php echo $_ENV[\"SERVER_SOFTWARE\"] ?> <br> \
			Server Protocol:  <?php echo $_ENV[\"SERVER_PROTOCOL\"] ?> <br> \
			CGI Revision:     <?php echo $_ENV[\"GATEWAY_INTERFACE\"] ?> <br> \
			<HR></PRE> \
		</BODY> \
	</HTML>";
	write(fdIn, body.c_str(), body.size());
	lseek(fdIn, 0, SEEK_SET);

	if ((pid = fork()) == -1)
	{
		std::cerr << "Error: fork failed, Status: 500 " << '\n';
	}
	else if (pid == 0)
	{
		char * const * nll = NULL;

		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve(cgiPath.c_str(), nll, cgiEnv);
	}
	else
	{
		char	buffer[1025] = {0};

		waitpid(0, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);

		int ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, 1025);
			ret = read(fdOut, buffer, 1024);
			newBody += buffer;
		}
	}

	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	fclose(fIn);
	fclose(fOut);
	close(fdIn);
	close(fdOut);
	close(saveStdin);
	close(saveStdout);

	for (int i = 0;  cgiEnv[i]; i++)
		delete cgiEnv[i];
	delete [] cgiEnv;
	if (pid == 0)
		exit(0);
	std::cout << newBody << '\n';
	return (0);
}
