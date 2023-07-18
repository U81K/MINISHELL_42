/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exucution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 16:04:36 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/18 14:49:56 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int is_path(t_cmd cmd)
{
    struct stat    buf;

    if(cmd.full_cmd[0][0] == '/' || cmd.full_cmd[0][0] == '.')
    {
        if (access(cmd.full_cmd[0], F_OK) == 0 && stat(cmd.full_cmd[0], &buf) == 0)
        {
            if (access(cmd.full_cmd[0], X_OK) == -1)
                return (to_write_error(cmd.full_cmd[0], " : Permission denied\n", 21, 126), 0);
            else if (S_ISDIR(buf.st_mode))
                return (to_write_error(cmd.full_cmd[0], " : is a directory\n", 18, 126), 0);
            else
                return 1;
        }
        else
            return (to_write_error(cmd.full_cmd[0], " : No such file or directory\n", 29, 127), 0);
    }
    return 2;
}


t_tool *get_info(t_tool *tool, t_env *environ)
{
    
    tool->env = from_list_to_tab(environ);
    tool->path = find_path(tool->env, 4, "PATH");
    tool->paths = ft_split(tool->path, ':');
    tool->pid = 0;
    tool->handler = 0;
    tool->i = 0;
    return tool;
}


void search_and_exece(t_tool *tool, t_cmd *cmd)
{
    char *cmd_slash;
    char *cmd_;
    tool->i = 0;
    while (tool->paths[tool->i])
    {
        cmd_slash = ft_strjoin(tool->paths[tool->i++], "/");
        cmd_ = ft_strjoin(cmd_slash, cmd->full_cmd[0]);
        free(cmd_slash);
        if(execve(cmd_, cmd->full_cmd, tool->env) == -1)
            free(cmd_);
    }
    write(2, "my_Shell: : command not found\n", 30);
    exist_status = 127;
    exit(exist_status);
}

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
