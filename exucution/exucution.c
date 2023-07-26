/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exucution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 16:04:36 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/21 23:40:33 by ybourais         ###   ########.fr       */
=======
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 16:04:36 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/21 22:48:45 by bgannoun         ###   ########.fr       */
>>>>>>> de50f55b4e6e55a5ecb788d6702b1e3a496376b2
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_path(t_cmd cmd)
{
	struct stat	buf;

	if (!cmd.full_cmd)
		return (-1);
	if (cmd.full_cmd[0][0] == '/' || cmd.full_cmd[0][0] == '.')
	{
		if (access(cmd.full_cmd[0], F_OK) == 0 && stat(cmd.full_cmd[0],
				&buf) == 0)
		{
			if (access(cmd.full_cmd[0], X_OK) == -1)
				return (to_write_error(cmd.full_cmd[0],
						" : Permission denied\n", 21, 126), 0);
			else if (S_ISDIR(buf.st_mode))
				return (to_write_error(cmd.full_cmd[0], " : is a directory\n",
						18, 126), 0);
			else
				return (1);
		}
		else
			return (to_write_error(cmd.full_cmd[0],
					" : No such file or directory\n", 29, 127), 0);
	}
	return (2);
}

void	search_and_exece(t_tool *tool, t_cmd *cmd)
{
	char	*cmd_slash;
	char	*cmd_;

	tool->i = 0;
	if (!tool->paths)
	{
		write(2, "my_Shell: : command not found\n", 30);
		g_exit_status = 127;
		exit(g_exit_status);
	}
	while (tool->paths[tool->i])
	{
		cmd_slash = ft_strjoin(tool->paths[tool->i++], "/");
		cmd_ = ft_strjoin(cmd_slash, cmd->full_cmd[0]);
		free(cmd_slash);
		if (execve(cmd_, cmd->full_cmd, tool->env) == -1)
			free(cmd_);
	}
	write(2, "my_Shell: : command not found\n", 30);
	g_exit_status = 127;
	exit(g_exit_status);
}

void	single_command(t_cmd cmd, t_tool *tool)
{
	signal(SIGQUIT, SIG_DFL);
	handle_herdoc(cmd);
	handle_redirection(cmd);
	tool->handler = is_path(cmd);
	if (tool->handler == 1)
		execve(cmd.full_cmd[0], cmd.full_cmd, tool->env);
	else if (tool->handler == 2)
		search_and_exece(tool, &cmd);
	else
		exit(g_exit_status);
}

void	exucution(t_cmd cmd, t_env *environ)
{
	t_tool	*tool;

	tool = malloc(sizeof(t_tool));
	get_info(tool, environ);
	if (cmd.nbr_cmd <= 1)
	{
		tool->pid = fork();
		if (tool->pid == 0)
			single_command(cmd, tool);
		free_and_wait(tool);
	}
	else
	{
		handle_herdoc(cmd);
		handle_redirection(cmd);
		tool->handler = is_path(cmd);
		if (tool->handler == 1)
			execve(cmd.full_cmd[0], cmd.full_cmd, tool->env);
		else if (tool->handler == 2)
			search_and_exece(tool, &cmd);
		else
			exit(g_exit_status);
	}
}

t_env	*commands(t_cmd *cmd, t_env *env, t_info *info)
{
	(void)info;
	if (!cmd->full_cmd)
		exucution(*cmd, env);
	else if (compare(cmd->full_cmd[0], "export"))
		env = ft_export(*cmd, env);
	else if (compare(cmd->full_cmd[0], "env"))
		print_env(env, cmd);
	else if (compare(cmd->full_cmd[0], "pwd"))
		pwd(cmd);
	else if (compare(cmd->full_cmd[0], "cd"))
		cd(cmd->full_cmd, env);
	else if (compare(cmd->full_cmd[0], "unset"))
		env = unset(*cmd, env);
	else if (compare(cmd->full_cmd[0], "echo"))
		echo(cmd->full_cmd, cmd);
	else if (compare(cmd->full_cmd[0], "exit"))
		ft_exit(cmd->full_cmd);
	else
		exucution(*cmd, env);
	return (env);
}
