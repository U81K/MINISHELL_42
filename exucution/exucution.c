/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exucution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 16:04:36 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/19 19:25:50 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int is_path(t_cmd cmd)
{
    struct stat    buf;

    if(!cmd.full_cmd)
        return -1;
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
        t_rd *curr;
        curr = cmd.rd;
        while (curr)
        {
            if (curr->type == R_OUT) // >
            {
                int fd = open(curr->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if(fd == -1)
                {
                    write(2, "my_Shell: ", 10);
                    write(2, curr->file, ft_strlen(curr->file));
                    write(2, ": No such file or directory\n", 28);
                    exist_status = 1;
                    exit(exist_status);
                }
                else
                {   
                    dup2(fd, 1);
                    close(fd);
                }
            }
            else if (curr->type == DR_OUT) // >>
            {
                int fd = open(curr->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
                if(fd == -1)
                {
                    write(2, "my_Shell: ", 10);
                    write(2, curr->file, ft_strlen(curr->file));
                    write(2, ": No such file or directory\n", 28);
                    exist_status = 1;
                    exit(exist_status);
                }
                else
                {   
                    dup2(fd, 1);
                    close(fd);
                }
            }
            else if (curr->type == R_IN) // < // ? if the file exist
            {
                int fd = open(curr->file, O_RDONLY);
                if(fd == -1)
                {
                    write(2, "my_Shell: ", 10);
                    write(2, curr->file, ft_strlen(curr->file));
                    write(2, ": No such file or directory\n", 28);
                    exist_status = 1;
                    exit(exist_status);
                }
                else
                {   
                    dup2(fd, 0);
                    close(fd);
                }
            }
            curr = curr->next;
        }
        t_rd *cur;
        cur = cmd.rd;
        int fd = -1;
        while (cur)
        {
            if (cur && cur->type == DR_IN) // <<
            {
                char *in;
                char *path;
                char *line;
                int tmp = dup(1);
                dup2(cmd.old_out, 1);
                path = ft_strjoin("/tmp/", cur->file);
                
                unlink(path);
                fd = open(path, O_CREAT | O_RDWR | O_APPEND, 0644);
                while (1)
                {
                    in = readline("> ");
                    if (compare(in, cur->file))
                    {
                        free(in);
                        break;
                    }
                    line = ft_strjoin(in, "\n");
                    ft_putstr_fd(line, fd);
                    free(line);
                    free(in);
                }
                close(fd);
                fd = open(ft_strjoin("/tmp/", cur->file), O_RDWR, 0644);
                if (cur->next)
                    close(fd);
                free(path);
                dup2(tmp,1);
            }
            cur = cur->next;
        }
        if (fd != -1)
        {
            dup2(fd, 0);
            close(fd);
        }
        tool->handler = is_path(cmd);
        if(tool->handler == 1) /* if the file exist and have all permisiion */
            execve(cmd.full_cmd[0], cmd.full_cmd, tool->env);
        else if(tool->handler == 2)
            search_and_exece(tool, &cmd);
        else
            exit(exist_status);
    }
    free_and_wait(tool);
}

t_env *commands(t_cmd *cmd, t_env* env, t_info *info)
{
    (void)info;
    if(!cmd->full_cmd)
        exucution(*cmd, env);
    else if (compare(cmd->full_cmd[0], "export"))
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
