/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exucution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 16:04:36 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/18 11:56:17 by ybourais         ###   ########.fr       */
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
            {
                write(2, cmd.full_cmd[0], ft_strlen(cmd.full_cmd[0]));
                write(2, " : Permission denied\n", 21);
                exist_status = 126;
                return 0;
            }
            else if (S_ISDIR(buf.st_mode))
            {
                write(2, cmd.full_cmd[0], ft_strlen(cmd.full_cmd[0]));
                write(2, " : is a directory\n", 18);
                exist_status = 126;
                return 0;
            }
            else
                return 1;
        }
        else
        {
            write(2, cmd.full_cmd[0], ft_strlen(cmd.full_cmd[0]));
            write(2, " : No such file or directory\n", 29);
            exist_status = 127;
            return 0;
        } 
    }
    return 2;
}

void exucution(t_cmd cmd, t_env *environ)
{
    int i;
    char **env;
    char **paths;
    char *path;

    env = from_list_to_tab(environ);
    path = find_path(env, 4, "PATH");
    paths = ft_split(path, ':');
    int pid = fork();

    if(pid == 0)
    {
        int handler = is_path(cmd);
        if(handler == 1)
            execve(cmd.full_cmd[0], cmd.full_cmd, env);
        else if(handler == 2)
        {
            if(cmd.full_cmd[0][0] == '\0')
            {
                write(2, " command not found\n", 18);
                exit(1);
            }
            else
            {
                i = 0;
                while (paths[i])
                {
                    
                    char *cmd_slash = ft_strjoin(paths[i], "/");
                    char *cmd_ = ft_strjoin(cmd_slash, cmd.full_cmd[0]);
                    free(cmd_slash);
                    if(execve(cmd_, cmd.full_cmd, env) == -1)
                        free(cmd_);
                    i ++;
                }
                write(2, "my_Shell: : command not found\n", 30);
                exist_status = 127;
                exit(exist_status);
            }
        }
        else
            exit(exist_status);
    }
    int h;
    waitpid(pid, &h, 0);
    exist_status = WEXITSTATUS(h);
    free_tab(env);
    free_tab(paths);
    free(path);
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

