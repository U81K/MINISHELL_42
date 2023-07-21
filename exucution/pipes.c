/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 09:49:49 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/21 20:19:48 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	redirect_fd_to_pipe_and_close(int num_of_cmd, int (*fd)[2], int index)
{
	if (index == 0)
	{
		dup2(fd[index][1], STDOUT_FILENO);
		close_fd(num_of_cmd, fd);
	}
	else if (index == num_of_cmd - 1)
	{
		dup2(fd[index - 1][0], STDIN_FILENO);
		close_fd(num_of_cmd, fd);
	}
	else
	{
		dup2(fd[index - 1][0], STDIN_FILENO);
		dup2(fd[index][1], STDOUT_FILENO);
		close_fd(num_of_cmd, fd);
	}
}

t_env	*fork_multiple_command(t_cmd *cmd, t_env *env, t_info *info, int num)
{
	int	(*fd)[2];
	int	*pid;
	int	i;

	pid = malloc(sizeof(int) * num);
	fd = malloc(sizeof(int) * num * 2);
	creat_pipes(num, fd);
	i = 0;
	while (i < num)
	{
		pid[i] = fork();
		if (pid[i] == 0)
		{
			cmd[i].old_in = dup(STDIN_FILENO);
			cmd[i].old_out = dup(STDOUT_FILENO);
			redirect_fd_to_pipe_and_close(num, fd, i);
			env = commands(&cmd[i], env, info);
			exit(exist_status);
		}
		i++;
	}
	close_fd(num, fd);
	wait_for_child(num, fd, pid);
}

t_env	*run_commands(t_cmd *cmd, t_env *env, t_info *info)
{
	int	num_c;

	num_c = nbr_cmd(info);
	num_c = cmd[0].nbr_cmd;
	if (num_c <= 1)
	{
		cmd[0].old_in = dup(STDIN_FILENO);
		cmd[0].old_out = dup(STDOUT_FILENO);
		env = commands(&cmd[0], env, info);
	}
	else
		env = fork_multiple_command(cmd, env, info, num_c);
	return (env);
}
