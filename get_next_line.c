/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ragolden <ragolden@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:17:07 by ragolden          #+#    #+#             */
/*   Updated: 2026/02/09 16:55:02 by ragolden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*clear(char *buf, char *stash) //ajouter le contenu du buff au stash
{
	char	*old_stash;

	old_stash = stash;
	stash = ft_strjoin(old_stash, buf); //concatenne old stash et contenu lu dans le buf
	free(old_stash);//libere lancien 
	return (stash);//retourne le nouveau
}

char	*extract_line(char **stash) //recupere une ligne complete et met a jour le stash pour quil ne contienne plus ce quil reste
{
	char	*line;
	char	*tmp;
	int		i;

	if (!*stash || !**stash) //verifier si le stash existe
		return (NULL);
	i = 0;
	while ((*stash)[i] && (*stash)[i] != '\n') //avance jusqua trouver un \n ou jusqua la fin de la ligne
		i++;
	if ((*stash)[i] == '\n') //si on tombe sur un \n == ligne complete 
	{
		line = ft_substr(*stash, 0, i + 1);//du debut a la fin
		tmp = ft_substr(*stash, i + 1, ft_strlen(*stash) - i);//ce qu il reste apres la ligne
		free(*stash);
		*stash = tmp;//remplace stash par tmp
		return (line);//renvoie line
	}
	//cas ou ya pas de \n
	line = ft_strdup(*stash); //derniere ligne du fichier
	free(*stash);//vide totalement stash
	*stash = NULL;
	return (line);//retourne le stash comme ligne
}

static char	*read_and_store(int fd, char *stash)//lis dans le fichier jusqua ce quon trouve un \n ou a la fin
{
	char	*buffer;
	int		bytes_read;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1)); //alloue la taille au buffer
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (bytes_read > 0)//boucle de lecture
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);//lis
		if (bytes_read < 0)//gestion des erreurs
		{
			free(stash);
			free(buffer);
			return (NULL);
		}
		buffer[bytes_read] = '\0';//terminer la chaine
		stash = clear(buffer, stash);//ajouter au stash
		if (stash && ft_strchr(stash, '\n'))//si ya un \n on arrete de lire
			break ;
	}
	free(buffer);//nettoyage
	return (stash);//retourne la stash complete 
}

char	*get_next_line(int fd)//lire le fichier extraire une ligne et la renvoyer
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)//verif de base
		return (NULL);
	stash = read_and_store(fd, stash);//lire et remplir le stash
	if (!stash)
		return (NULL);
	line = extract_line(&stash);//extraire une ligne du stash
	if (!line && stash)//nettoryer en fin de fichier
	{
		free(stash);
		stash = NULL;
	}
	return (line);
}

#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("/dev/random", O_RDONLY);
	while ((line = get_next_line(fd)))
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
}

// 	int		fd1;
// 	char	*line1;
// 	fd1 = open("test.txt", O_RDONLY);
// 	while ((line1 = get_next_line(fd1)))
// 	{
// 			printf("%s\n", line1);
// 			free(line1);
// 	}
// 	line1 = get_next_line(fd1);
// 	printf("%s\n", line1);
//  	close(fd1);
// }