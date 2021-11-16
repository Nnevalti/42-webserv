/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 19:16:02 by sgah              #+#    #+#             */
/*   Updated: 2021/07/06 02:18:17 by sgah             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.hpp"

int
	ft_strlen(char *str)
{
	int i = 0;

	while(str[i])
		i++;
	return i;
}

char
	*ft_strdup(char *str)
{
	int i = ft_strlen(str);
	char *dup;

	if ((dup = (char *)malloc(sizeof(char) * (i + 1))) == NULL)
		return NULL;
	i = 0;
	while(str[i])
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return dup;
}

char
	*ft_strjoinfree(char **line, char *str)
{
	int i = ft_strlen(*line);
	int j = ft_strlen(str);
	char *ret;

	if ((ret = (char *)malloc(sizeof(char) * (i + j + 1))) == NULL)
		return NULL;
	i = 0;
	j = 0;
	while ((*line)[i])
	{
		ret[i] = (*line)[i];
		i++;
	}
	while (str[j])
	{
		ret[i] = str[j];
		j++;
		i++;
	}
	ret[i] = '\0';
	free(*line);
	return ret;
}

int
	read_line(int fd, char *stack, char *buf)
{
	int len;

	if ((len = read(0, buf, 0)) != 0)
		return len;
	if (stack)
	{
		while(stack[len])
		{
			buf[len] = stack[len];
			len++;
		}
		buf[len] = '\0';
		free(stack);
		return len;
	}
	len = recv(fd, buf, BUFFER_SIZE, 0);
	buf[len] = '\0';
	return len;
}

int
	get_next_line(int fd, char **line)
{
	static char	*stack;
	char		buf[BUFFER_SIZE + 1];
	int		i;
	int		join;
	int		len;
	int		gl;

	if (!line)
		return -1;
	join = 0;
	while ((len = read_line(fd, stack, buf)) > 0)
	{
		stack = NULL;
		gl = 0;
		i = 0;
		while (i < len && buf[i] != '\n')
			i++;
		if (buf[i] == '\n')
		{
			gl = 1;
			buf[i] = '\0';
			i++;
			if (buf[i])
				stack = ft_strdup(buf + i);
		}
		*line = (join) ? ft_strjoinfree(line, buf) : ft_strdup(buf);
		join = 1;
		if (gl == 1)
			return 1;
	}
	if (*line == NULL)
		*line = ft_strdup((char *)"");
	return len;
}

