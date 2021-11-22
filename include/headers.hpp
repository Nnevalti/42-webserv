/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 16:07:48 by sgah              #+#    #+#             */
/*   Updated: 2021/11/22 19:55:17 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

//* C LIBRARY
# include <unistd.h>
# include <errno.h>
# include <dirent.h>
# include <fcntl.h>

//* C NETWORK
# include <sys/types.h>
# include <sys/time.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <sys/poll.h>
# include <arpa/inet.h>
# include <netinet/in.h>

//* CPP LIBRARY
# include <iostream>
# include <sstream>
# include <cstring>
# include <cstdlib>
# include <limits>
# include <cctype>
# include <fstream>
# include <string>
# include <stdexcept>


//* STANDARD TEMPLATE LIBRARY
# include <map>
# include <set>
# include <vector>
# include <algorithm>
# include <iterator>
# include <list>

//* COLORS
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define END "\033[0m"

// *STRUCT
typedef struct	s_network
{
	unsigned int	host;
	int				port;
}				t_network;

//* SHORTCUT
# define BUFFER_SIZE 512
# define stringVector std::vector<std::string>
# define stringMap std::map<std::string, std::string>

