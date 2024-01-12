/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaerema <nlaerema@student.42lehavre.fr>	+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 10:58:17 by nlaerema          #+#    #+#             */
/*   Updated: 2024/01/12 13:37:53 by nlaerema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	is_valid_number(char *line)
{
	int		index;
	char	*end;

	index = strtol(line, &end, 10);
	if (*end)
		return (0);
	return (1);
}

int	open_file(t_file *file, char *file_name)
{
	t_list	*current_line;
	size_t	line_len;
	char	*line;
	int		fd;

	file->line = NULL;
	file->current_line = 0;
	file->file_name = file_name;
	fd = open(file_name, O_RDONLY | O_CREAT, 0644);
	if (fd < 0)
		return (EXIT_FAILURE);
	line = ft_gnl(fd).buf;
	while (line)
	{
		line_len = strlen(line);
		if (line[line_len - 1] == '\n')
			line[line_len - 1] = '\0';
		current_line = ft_lstnew(line);
		ft_lstadd_back(&file->line, current_line);
		line = ft_gnl(fd).buf;
	}
	current_line = ft_lstnew(strdup(""));
	ft_lstadd_back(&file->line, current_line);
	ft_close(&fd);
	return (EXIT_SUCCESS);
}

int	move_line(t_file *file, char *line)
{
	long	i;
	long	index;
	char	*end;
	t_list	**current_line;

	i = 0;
	current_line = &file->line;
	index = strtol(line + 1, &end, 10);
	if (index < 0)
		return (EXIT_FAILURE);
	while (i <= index)
	{
		if (!*current_line)
			*current_line = ft_lstnew(strdup(""));
		current_line = &(*current_line)->next;
		i++;
	}
	file->current_line = index;
	return (EXIT_SUCCESS);
}

int	print_line(t_file *file)
{
	t_list	*current_line;
	char	*line;

	current_line = ft_lstget(file->line, file->current_line);
	if (!current_line)
		return (EXIT_FAILURE);
	line = current_line->data;
	printf("%s\n", line);
	return (EXIT_SUCCESS);
}

int	write_line(t_file *file, char *line)
{
	t_list	*current_line;

	current_line = ft_lstget(file->line, file->current_line);
	if (!current_line)
		return (EXIT_FAILURE);
	free(current_line->data);
	current_line->data = line;
	return (EXIT_SUCCESS);
}

int	save_file(t_file *file)
{
	t_list	*current_line;
	char	*line;
	size_t	i;
	int		fd;

	i = 1;
	fd = open(file->file_name, O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (EXIT_FAILURE);
	current_line = file->line;
	while (current_line)
	{
		line = current_line->data;
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		current_line = current_line->next;
		i++;
	}
	close(fd);
	return (EXIT_SUCCESS);
}

int	main(void)
{
	static t_file	file = {};
	char			*file_name;
	char			*line;
	char			prompt[12];

	while (1)
	{
		file_name = readline("file name > ");
		if (open_file(&file, file_name))
		{
			perror("Can't open file");
		}
		else
		{
			while (1)
			{
				sprintf(prompt, "%ld > ", file.current_line);
				line = readline(prompt);
				if (!line || !strcmp(line, "EOF"))
					break ;
				else if (line[0] == 'm' && is_valid_number(line + 1))
				{
					if (move_line(&file, line))
						fprintf(stderr, "move error !\n");
				}
				else if (line[0] == 'p' && line[1] == '\0')
				{
					if (print_line(&file))
						fprintf(stderr, "print error !\n");
				}
				else if (line[0] == 's' && line[1] == '\0')
				{
					if (save_file(&file))
						fprintf(stderr, "save error !\n");
				}
				else
				{
					if (write_line(&file, line))
						fprintf(stderr, "write error !\n");
				}
			}
			ft_lstclear(&file.line, free);
		}
		free(file_name);
	}
}
