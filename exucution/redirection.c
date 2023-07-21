/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 11:32:20 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/21 20:36:04 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exucute_red_out(char *file_name)
{
	int	fd;

	fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("my_Shell");
		exist_status = 1;
		exit(exist_status);
	}
	else
	{
		dup2(fd, 1);
		close(fd);
	}
}

void	exucute_double_red_out(char *file_name)
{
	int	fd;

	fd = open(file_name, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("my_Shell");
		exist_status = 1;
		exit(exist_status);
	}
	else
	{
		dup2(fd, 1);
		close(fd);
	}
}

void	exucute_red_in(char *file_name)
{
	int	fd;

	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		perror("my_Shell");
		exist_status = 1;
		exit(exist_status);
	}
	else
	{
		dup2(fd, 0);
		close(fd);
	}
}

void	handle_redirection(t_cmd cmd)
{
	t_rd	*curr;

	curr = cmd.rd;
	while (curr)
	{
		if (curr->type == R_OUT)
			exucute_red_out(curr->file);
		else if (curr->type == DR_OUT)
			exucute_double_red_out(curr->file);
		else if (curr->type == R_IN)
			exucute_red_in(curr->file);
		curr = curr->next;
	}
}
