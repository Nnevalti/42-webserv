/**========================================================================
 **                           MINI CLIENT
 *?  The goal is to write a program that take an IP and request a GET
 *?  on the root to this IP on the network For example
 *?  to get the ip of www.google.com, ping the url
 *
 **  USAGE: ./mini_client [IP] [PORT]
 *@param [IP] string
 *@param [PORT] int
 *
 *@functions :
 *    socket
 *    memset
 *    inet_addr
 *    connect
 *    send
 *    recv
 *    close
 *
 *@behavior :
 * If a syscall fail, it will print the syscall involved and exit the program.
 * Else, it will display on STDOUT the FULL response that he recieved
 *========================================================================**/

#include "get_next_line.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <stdlib.h>
#include <fcntl.h>

# define GREEN "\033[0;32m"
# define RED "\033[0;31m"
# define YELLOW "\033[0;33m"
# define SET "\033[0m"

int main(int ac, char **av)
{
	struct sockaddr_in servaddr;
	int		sock;

/**========================================================================
 * ?                         USAGE ERROR MANAGE
 *========================================================================**/
	if(ac != 3)
	{
		std::cerr << "usage: ./mini_client [IP] [PORT]" << std::endl;
		exit(1);
	}

/**========================================================================
 * ?                         SOCKET CREATION
 *========================================================================**/
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "error: socket()" << std::endl;
		exit(1);
	}

/**========================================================================
 * ?                         FILL THE SOCKADRR STRUCT
 *========================================================================**/
	std::memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(std::atoi(av[2]));

/**========================================================================
 * ?                         TRANSLATE IP INTO BINARY
 *========================================================================**/
	servaddr.sin_addr.s_addr = inet_addr(av[1]);

/**========================================================================
 * ?                         CONNEXION TO THE ADDRESS
 *========================================================================**/
	if(connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
	{
		std::cerr << "error: connect()" << std::endl;
		exit(1);
	}

/**========================================================================
 * ?                         CREATE AND SEND HTTP REQUEST
 *========================================================================**/
	char sendline[] = "GET / HTTP/1.1\r\n\r\n";
	int sendlen = strlen(sendline);

	if (send(sock, sendline, sendlen, 0) < 0)
	{
		std::cerr << "error: send()" <<std::endl;
		exit(1);
	}

/**========================================================================
 * ?                         RECIEVE AND DISPLAY THE REQUEST
 *========================================================================**/
	int status = 0;
	char *buf;

	while ((status = get_next_line(sock, &buf)) > 0)
	{
		std::cout << buf << std::endl;
		free(buf);
	}

/**========================================================================
 * ?                         ERROR RECIEVE MANAGE
 *========================================================================**/
	if (status == -1)
	{
		std::cerr << "error: recv()"<< std::endl;
		exit(1);
	}

/**========================================================================
 * ?                         CLOSE THE SOCKET
 *========================================================================**/
	close(sock);

	return (0);
}
