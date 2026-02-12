/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ragolden <ragolden@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 11:00:00 by ragolden          #+#    #+#             */
/*   Updated: 2025/12/10 13:53:31 by ragolden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*clear(char *buf, char *stash)
{
	char	*old_stash;

	old_stash = stash;
	stash = ft_strjoin(old_stash, buf);
	free(old_stash);
	return (stash);
}

char	*extract_line(char **stash)
{
	char	*line;
	char	*tmp;
	int		i;

	if (!*stash || !**stash)
		return (NULL);
	i = 0;
	while ((*stash)[i] && (*stash)[i] != '\n')
		i++;
	if ((*stash)[i] == '\n')
	{
		line = ft_substr(*stash, 0, i + 1);
		tmp = ft_substr(*stash, i + 1, ft_strlen(*stash) - i);
		free(*stash);
		*stash = tmp;
		return (line);
	}
	line = ft_strdup(*stash);
	free(*stash);
	*stash = NULL;
	return (line);
}

static char	*read_and_store(int fd, char *stash)
{
	char	*buffer;
	int		bytes_read;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(stash);
			free(buffer);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		stash = clear(buffer, stash);
		if (stash && ft_strchr(stash, '\n'))
			break ;
	}
	free(buffer);
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash[MAX_FD];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= MAX_FD)
		return (NULL);
	stash[fd] = read_and_store(fd, stash[fd]);
	if (!stash[fd])
		return (NULL);
	line = extract_line(&stash[fd]);
	if (!line && stash[fd])
	{
		free(stash[fd]);
		stash[fd] = NULL;
	}
	return (line);
}
