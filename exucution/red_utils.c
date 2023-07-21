/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   red_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 20:20:21 by bgannoun          #+#    #+#             */
/*   Updated: 2023/07/21 20:35:21 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_heredoc_2(t_rd *cur, int fd)
{
	char	*in;
	char	*line;

	in = readline("> ");
	if (compare(in, cur->file))
	{
		free(in);
		return (1);
	}
	line = ft_strjoin(in, "\n");
	ft_putstr_fd(line, fd);
	free(line);
	free(in);
	return (0);
}

void	handle_heredoc_1(t_rd *cur, t_cmd cmd, int fd)
{
	int		tmp;
	int		tmp_in;
	char	*path;

	tmp = dup(1);
	tmp_in = dup(0);
	dup2(cmd.old_out, 1);
	dup2(cmd.old_in, 0);
	path = ft_strjoin("/tmp/", cur->file);
	unlink(path);
	fd = open(path, O_CREAT | O_RDWR | O_TRUNC, 0644);
	while (1)
		if (handle_heredoc_2(cur, fd) == 1)
			break ;
	close(fd);
	fd = open(ft_strjoin("/tmp/", cur->file), O_RDWR, 0644);
	free(path);
	dup2(tmp, 1);
	dup2(tmp_in, 0);
}

void	handle_herdoc(t_cmd cmd)
{
	t_rd	*cur;
	int		fd;

	cur = cmd.rd;
	fd = -1;
	while (cur)
	{
		if (cur && cur->type == DR_IN)
			handle_heredoc_1(cur, cmd, fd);
		cur = cur->next;
	}
	dup2(fd, 0);
	close(fd);
}
