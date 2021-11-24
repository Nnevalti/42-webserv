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

typedef std::vector<int> fd_vector;
fd_vector servers_fd;

void signal_handler(int signum)
{
	std::cout << "\r" << "(" << signum << ") Serveur ending...       " << std::endl;
	for (fd_vector::iterator it = servers_fd.begin(); it != servers_fd.end(); it++)
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
	fd_set select_set_read;
	fd_set select_set_read_ready;
	struct timeval select_timeout;

	char request[1024];
	std::string response;
	response += "HTTP/1.1 200 OK\n";
	response += "Content-Type: text/html\r\n";
	response += "Content-Length: 13\n\n";
	response += "Hello World !\r\n\r\n";

	int new_socket = 0;
	int status;
	int max_fd;

	signal(SIGINT, signal_handler);

	if (ac < 2)
	{
		std::cerr << "USAGE: ./select [PORT]" << std::endl;
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
		status = 0;

		for (fd_vector::iterator it = servers_fd.begin(); it != servers_fd.end(); it++)
			FD_SET(*it, &select_set_read);

/**========================================================================
 * *                                SERVER LOOP
 * ? FD_SET: ADD LISTEN_FD TO THE SET
 * ? FD_ISSET: CHECK IF LISTEN_FD IS IN THE SET AFTER SELECT CALL
 * ? ACCEPT: CREATE NEW SOCKET
 * ? SEND: ANSWER TO CLIENT REQUEST
 * ? CLOSE: CLOSE THE SOCKET
 *========================================================================**/
 std::string  wait[] = {"⠋", "⠙", "⠸", "⠴", "⠦", "⠇"};
	int n = 0;

	while (true)
	{
		errno = 0;

		while (status == 0)
		{
			// reset fd_set
			FD_ZERO(&select_set_read_ready);
			select_set_read_ready = select_set_read;

			std::cout << "\r" << wait[(n++ % 6)] << GREEN << " waiting for connection" << SET << std::flush;
			usleep(100000);

			// Verify if a new connection is available
			if ((status = select(max_fd + 1, &select_set_read_ready, NULL, NULL, &select_timeout)) < 0)
			{
				std::cerr << "error: select()";
				exit(1);
			}
		}

		if (status > 0)
		{
			// Verify which server has a new connection
			for (fd_vector::iterator it = servers_fd.begin(); it != servers_fd.end(); it++)
			{
				// If the server has a new connection ready
				if (FD_ISSET(*it, &select_set_read_ready))
				{
						std::cout << "\r" << "Client connected on server: " << *it << std::endl;
						// Accept the connection
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
							int ret = 0;

							// Receive the request
							if ((ret = recv(new_socket, &request, 1023, 0)) < 0)
							{
								std::cerr << "error: recv()" << std::endl;
								exit(1);
							}
							else
							{
								request[ret] = '\0';
								std::cout << request << std::endl;
							}

							// send the response
							if(send(new_socket, response.c_str(), response.size(), 0) < 0) {
								std::cerr << "error: send()" << std::endl;
								exit(1);
							}
							// close the socket
							close(new_socket);
						}
					}
				}

			}
			status = 0;
	}
	return (0);
}
