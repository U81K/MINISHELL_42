/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 09:50:08 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/21 20:19:00 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	wait_for_child(int numb_of_cmd, int (*fd)[2], int *pid)
{
	int	h;
	int	i;

	i = 0;
	while (i < numb_of_cmd)
	{
		waitpid(pid[i], &h, 0);
		if (i == numb_of_cmd - 1)
			exist_status = WEXITSTATUS(h);
		i++;
	}
	free(pid);
	free(fd);
}

void	creat_pipes(int numb_of_cmd, int (*fd)[2])
{
	int	i;

	i = 0;
	while (i < numb_of_cmd)
	{
		pipe(fd[i]);
		i++;
	}
}

void	close_fd(int num, int (*fd)[2])
{
	int	i;

	i = 0;
	while (i < num)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i++;
	}
}
