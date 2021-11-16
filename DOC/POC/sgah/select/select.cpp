/**========================================================================
 **                           SELECT
 *?  The goal of this Proof Of Concept is to reach the pros of asynchronous
 *?  programming.
 *?  Select will give the power to check if a socket is ready to read in it.
 *
 **  USAGE: ./select [PORT]
 *@param [PORT] int
 *
 *@functions :
 *    socket
 *    fcntl
 *    bind
 *    FD_ZERO
 *    FD_SET
 *    select
 *    FD_ISSET
 *    listen
 *    accept
 *    send
 *    close
 *
 *@behavion
 *  This is simple, a program that print ""No pending connections;"
 *  every seconds. But when a client is trying to connect to your program,
 *  it print "*CLIENT GET CONNECT*" on stdout.
 *========================================================================**/

#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

int listen_fd;

void signal_handler(int signum)
{
	std::cout << "(" << signum << ") Serveur ending..." << std::endl;
	close(listen_fd);
	exit(1);
}

int main(int ac, char **av)
{
	struct sockaddr_in servaddr;
	struct timeval select_timeout;
	fd_set select_set;
	std::string response;
	response += "HTTP/1.1 200 OK\n";
	response += "Content-Length: 13\n\n";
	response += "Hello World !\r\n\r\n";
	int new_socket = 0;
	int status;

	signal(SIGINT, signal_handler);

	if (ac != 2)
	{
		std::cerr << "USAGE: ./mini_serv [PORT]" << std::endl;
		exit(1);
	}

/**========================================================================
 * ?                         CREATE SOCKET
 *========================================================================**/
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "error: socket()" << std::endl;
		exit(1);
	}

/**========================================================================
 * ?                         TURN SOCKET INTO NON BLOCKING
 *========================================================================**/
	if(fcntl(listen_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "could not set socket to be non blocking" << std::endl;
		exit(1);
	}

/**========================================================================
 * ?                         FILL SOCKADDR STRUCT
 *========================================================================**/
	std::memset((char*)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(std::atoi(av[1]));

/**========================================================================
 * ?                         IDENTIFY SOCKET
 *========================================================================**/
	if (bind(listen_fd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
	{
		std::cerr << "error: bind()" << std::endl;
		exit(1);
	}

/**========================================================================
 * ?                         LISTEN SOCKET
 *========================================================================**/
	if (listen(listen_fd, 100) < 0)
	{
		std::cerr << "error: listen()" << std::endl;
		exit(1);
	}

/**========================================================================
 * ?                         INIT FD SET
 *========================================================================**/
	select_timeout.tv_sec = 1;
	select_timeout.tv_usec = 0;
	FD_ZERO(&select_set);
	status = 0;

/**========================================================================
 * *                                SERVER LOOP
 * ? FD_SET: ADD LISTEN_FD TO THE SET
 * ? FD_ISSET: CHECK IF LISTEN_FD IS IN THE SET AFTER SELECT CALL
 * ? ACCEPT: CREATE NEW SOCKET
 * ? SEND: ANSWER TO CLIENT REQUEST
 * ? CLOSE: CLOSE THE SOCKET
 *========================================================================**/
	while (true)
	{
		errno = 0;

		FD_SET(listen_fd, &select_set);

		if ((status = select(listen_fd + 1, &select_set, NULL, NULL, &select_timeout)) < 0) {
			std::cerr << "error: select()";
			exit(1);
		}

		if (status > 0 && FD_ISSET(listen_fd, &select_set)) {
			std::cout << "CLIENT GET CONNECT" <<std::endl;
			if ((new_socket = accept(listen_fd, NULL, NULL)) < 0) {
				if(errno != EWOULDBLOCK) {
					std::cerr << "error: accept()" << std::endl;
					exit(1);
				}
			}
			else {
				if(send(new_socket, response.c_str(), response.size(), 0) < 0) {
					std::cerr << "error: send()" << std::endl;
					exit(1);
				}
			}
		}
		else {
			std::cerr << "No pending connections; sleeping for one second." << std::endl;
			sleep(1);
		}
		close(new_socket);
	}
	return (0);
}
