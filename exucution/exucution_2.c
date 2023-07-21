/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exucution_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 13:34:06 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/21 22:48:32 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exist_or_not(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (0);
}

void	to_write_error(char *cmd, char *msg, int len, int exit_status)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, msg, len);
	g_exit_status = exit_status;
}

void	free_and_wait(t_tool *tool)
{
	int	h;

	waitpid(tool->pid, &h, 0);
	g_exit_status = WEXITSTATUS(h);
	if (tool->paths)
		free_tab(tool->paths);
	free_tab(tool->env);
	if (tool->path)
		free(tool->path);
	free(tool);
}

t_tool	*get_info(t_tool *tool, t_env *environ)
{
	tool->env = from_list_to_tab(environ);
	tool->path = find_path(tool->env, 4, "PATH");
	tool->paths = ft_split(tool->path, ':');
	tool->pid = 0;
	tool->handler = 0;
	tool->i = 0;
	return (tool);
}
