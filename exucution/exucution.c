/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exucution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 16:04:36 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/18 13:38:09 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void exucution(t_cmd cmd, t_env *environ)
{
    t_tool *tool;

    tool = malloc(sizeof(t_tool));
    get_info(tool, environ);
    tool->pid = fork();
    if(tool->pid == 0)
    {
        tool->handler = is_path(cmd);
        if(tool->handler == 1) /* if the file exist and have all permisiion */
            execve(cmd.full_cmd[0], cmd.full_cmd, tool->env);
        else if(tool->handler == 2)
            search_and_exece(tool, &cmd);
        else
            exit(exist_status);
    }
    free_and_wait(tool->pid, tool->env, tool->paths, tool->path);
}

int exist_or_not(char *str, char c)
{
    int i = 0;
    while (str[i])
    {
        if(str[i] == c)
            return i;
        i++;
    }
    return 0;
}

t_env *commands(t_cmd *cmd, t_env* env, t_info *info)
{
    (void)info;
    if (compare(cmd->full_cmd[0], "export"))
        env = ft_export(*cmd, env);
    else if (compare(cmd->full_cmd[0], "env"))
        print_env(env);
    else if (compare(cmd->full_cmd[0], "pwd"))
        pwd();
    else if (compare(cmd->full_cmd[0], "cd"))
        cd(cmd->full_cmd, env);
    else if (compare(cmd->full_cmd[0], "unset"))
        env = unset(*cmd, env);
    else if (compare(cmd->full_cmd[0], "echo"))
        echo(cmd->full_cmd);
    else if (compare(cmd->full_cmd[0], "exit"))
        ft_exit(cmd->full_cmd);
    else
        exucution(*cmd, env);
    return env;
}

