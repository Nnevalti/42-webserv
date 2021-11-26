/**========================================================================
 **                           POLL
 *?  The goal of this Proof Of Concept is to reach the pros of asynchronous
 *?  programming.
 *?  Poll will give the power to check if a socket is ready to read in it.
 *
 **  USAGE: ./poll [PORT]
 *@param [PORT] int
 *
 *@functions :
 *    socket
 *    setsockopt
 *    fcntl
 *    bind
 *    listen
 *    poll
 *    accept
 *    send
 *    close
 *
 *@behavion
 *  This is simple, a program that print ""No pending connections;"
 *  every seconds. But when a client is trying to connect to your program,
 *  it print "*Client connected on server: server_fd*" on stdout.
 *========================================================================**/

#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <poll.h>

#include <vector>
#include <algorithm>

# define GREEN "\033[0;32m"
# define RED "\033[0;31m"
# define YELLOW "\033[0;33m"
# define SET "\033[0m"

// vector to store the servers socket descriptor
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
	int on = 1;
	struct sockaddr_in servaddr;

	/*************************************************************/
	/*                      Create socket                        */
	/*************************************************************/
		if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			std::cerr << "error: socket()" << std::endl;
			exit(1);
		}

		/*************************************************************/
		/*       Allow socket descriptor to be reuseable             */
		/*************************************************************/
		if (setsockopt(listen_fd, SOL_SOCKET,  SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(int)) < 0)
		{
			std::cerr << "error: setsockopt() failed" << std::endl;
			close(listen_fd);
			exit(1);
		}

		/*************************************************************/
		/* Set socket to be nonblocking. All of the sockets for      */
		/* the incoming connections will also be nonblocking since   */
		/* they will inherit that state from the listening socket.   */
		/*************************************************************/
		if(fcntl(listen_fd, F_SETFL, O_NONBLOCK) < 0)
		{
			std::cerr << "could not set socket to be non blocking" << std::endl;
			exit(1);
		}

		/*************************************************************/
		/*               Fill the sock_addr_in struct                */
		/*************************************************************/
		std::memset((char*)&servaddr, 0, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = INADDR_ANY;
		servaddr.sin_port = htons(std::atoi(port));

		/*************************************************************/
		/*                       Bind the socket                     */
		/*************************************************************/
		if (bind(listen_fd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
		{
			std::cerr << "error: bind() failed" << std::endl;
			exit(1);
		}

		/*************************************************************/
		/*                       Listen the socket                   */
		/*************************************************************/
		if (listen(listen_fd, 100) < 0)
		{
			std::cerr << "error: listen() failed" << std::endl;
			exit(1);
		}

		return listen_fd;
}

int main(int ac, char **av)
{
	// Request buffer and response header (simple one)
	char request[1024];
	std::string response;
	response += "HTTP/1.1 200 OK\n";
	response += "Content-Type: text/html\r\n";
	response += "Content-Length: 13\n\n";
	response += "Hello World !\r\n\r\n";

	// struct pollfd used in poll()
	struct pollfd fds[ac - 1];
	signal(SIGINT, signal_handler);

	if (ac < 2)
	{
		std::cerr << "USAGE: ./poll [PORT]" << std::endl;
		exit(1);
	}

	/***********************************************************************/
	/* Creating the different socket listening on all the port provided   */
	/**********************************************************************/

	int i = 1;
	while (i < ac)
	{
		servers_fd.push_back(create_socket(av[i]));
		i++;
	}

 /*************************************************************/
 /*   Init struct poll fd with previously created sockets     */
 /*************************************************************/
		i = 0;
		for (fd_vector::iterator it = servers_fd.begin(); it != servers_fd.end(); it++)
		{
			fds[i].fd = *it; // set descriptor fd to to listening socket
			fds[i].events = 0; // Clear the bit array
			fds[i].events = fds[i].events | POLLIN; // Set the POLLIN bit
			i++;
		}

		// Loading array for a nice UI
		std::string  wait[] = {"⠋", "⠙", "⠸", "⠴", "⠦", "⠇"}; // array of frame
		int n = 0; // current frame

		/*************************************************************/
	  /*                        Server Loop                        */
	  /*************************************************************/
	int timeout = 500; // set timeout to 0.5 sec
	int status = 0;
	int new_socket = 0;
	int nb_server = servers_fd.size();

	while (true)
	{
		errno = 0;

		while (status == 0)
		{
			std::cout << "\r" << wait[(n++ % 6)] << GREEN << " waiting for connection" << SET << std::flush;

			// Verify if a new connection is available
			if ((status = poll(fds, nb_server, timeout)) < 0)
			{
				std::cerr << "error: poll() failed";
				exit(1);
			}
		}

		if (status > 0)
		{
			// Verify which server has a new connection
			for (int j = 0; j < nb_server; j++)
			{
				// If the server has a new connection ready
				if ((fds[j].revents & POLLIN) == POLLIN)
				{
						std::cout << "\r" << "Client connected on server: " << fds[j].fd << std::endl;
						// Accept the connection
						if ((new_socket = accept(fds[j].fd, NULL, NULL)) < 0)
						{
							if(errno != EWOULDBLOCK)
							{
								std::cerr << "error: accept() failed" << std::endl;
								exit(1);
							}
						}
						else
						{
							int ret = 0;

							// Receive the request
							if ((ret = recv(new_socket, &request, 1023, 0)) < 0)
							{
								std::cerr << "error: recv() failed" << std::endl;
								exit(1);
							}
							else
							{
								request[ret] = '\0';
								std::cout << request << std::endl;
							}

							// send the response
							if(send(new_socket, response.c_str(), response.size(), 0) < 0) {
								std::cerr << "error: send() failed" << std::endl;
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
