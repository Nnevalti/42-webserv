/**========================================================================
 **                           MINI SERVER
 *?  The goal of this program is to create a program that wait for a request
 *?  on a port and response always the same message.
 *
 **  USAGE: ./mini_serv [PORT]
 *@param [PORT] int
 *
 *@functions :
 *    socket
 *    fcntl
 *    bind
 *    listen
 *    accept
 *    recv
 *    send
 *    close
 *
 * @behavior :
 *  If a syscall fail, it will print the syscall involved and exit the program.
 *  Else, it will display on STDOUT the FULL response in ASCII that you recieved.
 *  Your server must response correctly to localhost:[PORT] by printing Hello World !
 *========================================================================**/

#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int ac, char **av)
{
	struct sockaddr_in servaddr;
	std::string response;
	response += "HTTP/1.1 200 OK\n";
	response += "Content-Length: 13\n\n";
	response += "Hello World !\r\n\r\n";
	int server_fd;
	int new_socket = 0;
	int len;

	if (ac != 2)
	{
		std::cerr << "USAGE: ./mini_serv [PORT]" << std::endl;
		exit(1);
	}

/**========================================================================
 * ?                         CREATE SOCKET
 *========================================================================**/
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "error: socket()" << std::endl;
		exit(1);
	}

/**========================================================================
 * ?                         TURN SOCKET INTO NON BLOCKING
 *========================================================================**/
	if(fcntl(server_fd, F_SETFL, O_NONBLOCK) < 0)
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
	if (bind(server_fd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
	{
		std::cerr << "error: bind()" << std::endl;
		exit(1);
	}

/**========================================================================
 * ?                         LISTEN SOCKET
 *========================================================================**/
	if (listen(server_fd, 100) < 0)
	{
		std::cerr << "error: listen()" << std::endl;
		exit(1);
	}

/**========================================================================
 * *                                SERVER LOOP
 * ? ACCEPT: CREATE NEW SOCKET
 * ? RECV: RECEIVE CLIENT REQUEST
 * ? SEND: ANSWER TO CLIENT REQUEST
 * ? CLOSE: CLOSE THE SOCKET
 *========================================================================**/
	while (true)
	{
		if ((new_socket = accept(server_fd, NULL, NULL)) < 0) {
			if (errno == EWOULDBLOCK) {
				std::cerr << "No pending connections; sleeping for one second." << std::endl;
				sleep(1);
			}
			else {
				std::cerr << "error: accept()" << std::endl;
				exit(1);
			}
		}
		else {
			char buffer[1024];

			if((len = recv(new_socket, buffer, sizeof buffer - 1, 0)) < 0){
				std::cerr << "error: recv()" << std::endl;
				exit(1);
			}
			buffer[len] = '\0';
			std::cout << buffer << std::endl;

			if(send(new_socket, response.c_str(), response.size(), 0) < 0) {
				std::cerr << "error: send()" << std::endl;
				exit(1);
			}
		}
		close(new_socket);
	}
	return (0);
}