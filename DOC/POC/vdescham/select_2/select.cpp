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

#include <vector>
#include <algorithm>

# define GREEN "\033[0;32m"
# define RED "\033[0;31m"
# define YELLOW "\033[0;33m"
# define SET "\033[0m"

typedef std::vector<int> server_vector;
server_vector servers_fd;

void signal_handler(int signum)
{
	std::cout << "(" << signum << ") Serveur ending..." << std::endl;
	for (server_vector::iterator it = servers_fd.begin(); it != servers_fd.end(); it++)
		close(*it);
	exit(1);
}

int create_socket(char *port)
{
	int listen_fd;
	struct sockaddr_in servaddr;

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
		servaddr.sin_port = htons(std::atoi(port));

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

		return listen_fd;
}

int main(int ac, char **av)
{
	// struct sockaddr_in servaddr;
	// struct timeval select_timeout;
	fd_set select_set_read;
	fd_set select_set_read_ready;
	// fd_set select_set_write;
	// fd_set select_set_write_ready;
	struct timeval select_timeout;

	std::string response;
	response += "HTTP/1.1 200 OK\n";
	response += "Content-Length: 13\n\n";
	response += "Hello World !\r\n\r\n";
	int new_socket = 0;
	int status;
	int max_fd;

	signal(SIGINT, signal_handler);

	if (ac < 2)
	{
		std::cerr << "USAGE: ./mini_serv [PORT]" << std::endl;
		exit(1);
	}

	int i = 1;
	while (i < ac)
	{
		servers_fd.push_back(create_socket(av[i]));
		max_fd = servers_fd.back();
		i++;
	}

/**========================================================================
 * ?                         INIT FD SET
 *========================================================================**/
		select_timeout.tv_sec = 1;
		select_timeout.tv_usec = 0;
		FD_ZERO(&select_set_read);
		FD_ZERO(&select_set_read_ready);
		status = 0;

		for (server_vector::iterator it = servers_fd.begin(); it != servers_fd.end(); it++)
			FD_SET(*it, &select_set_read);

/**========================================================================
 * *                                SERVER LOOP
 * ? FD_SET: ADD LISTEN_FD TO THE SET
 * ? FD_ISSET: CHECK IF LISTEN_FD IS IN THE SET AFTER SELECT CALL
 * ? ACCEPT: CREATE NEW SOCKET
 * ? SEND: ANSWER TO CLIENT REQUEST
 * ? CLOSE: CLOSE THE SOCKET
 *========================================================================**/
 std::string  wait[] = {"⠋ ", "⠙ ", "⠸ ",
												"⠴ ", "⠦ ", "⠇ "};
	int n = 0;
	while (true)
	{
		errno = 0;

		select_set_read_ready = select_set_read;
		// select_set_write_ready = select_set_write;

		if ((status = select(max_fd + 1, &select_set_read_ready, NULL, NULL, &select_timeout)) < 0)
		{
			std::cerr << "error: select()";
			exit(1);
		}

		for (server_vector::iterator it = servers_fd.begin(); it != servers_fd.end(); it++)
		{
			if (status > 0 && FD_ISSET(*it, &select_set_read_ready))
			{
				std::cout << "CLIENT GET CONNECT on server: " << *it << std::endl;
				if ((new_socket = accept(*it, NULL, NULL)) < 0)
				{
					if(errno != EWOULDBLOCK)
					{
						std::cerr << "error: accept()" << std::endl;
						exit(1);
					}
				}
				else
				{
					// max_fd = new_socket;
					if(send(new_socket, response.c_str(), response.size(), 0) < 0)
					{
						std::cerr << "error: send()" << std::endl;
						exit(1);
					}
				}
			}
			else
			{
				std::cout << "\r" << wait[(n++ % 6)] << GREEN <<"waiting for connection" << SET << std::flush;
				// std::cerr << "No pending connections; sleeping for one second." << std::endl;
				sleep(1);
			}
			close(new_socket);
		}
	}
	return (0);
}
