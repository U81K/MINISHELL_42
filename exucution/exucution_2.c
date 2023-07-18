/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exucution_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 13:34:06 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/18 13:35:32 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void to_write_error(char *cmd, char *msg, int len, int exit_status)
{
    write(2, cmd, ft_strlen(cmd));
    write(2, msg, len);
    exist_status = exit_status;
}

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

void free_and_wait(int pid, char **env, char **paths, char *path)
{
    int h;
    waitpid(pid, &h, 0);
    exist_status = WEXITSTATUS(h);
    free_tab(env);
    free_tab(paths);
    free(path);
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
