/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 11:32:20 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/21 13:53:37 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void exucute_red_out(char *file_name)
{
    int fd;

    fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if(fd == -1)
    {
        write(2, "my_Shell: ", 10);
        write(2, file_name, ft_strlen(file_name));
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

void exucute_double_red_out(char *file_name)
{
    int fd;

    fd = open(file_name, O_CREAT | O_WRONLY | O_APPEND, 0644);
    if(fd == -1)
    {
        write(2, "my_Shell: ", 10);
        write(2, file_name, ft_strlen(file_name));
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

void exucute_red_in(char *file_name)
{
    int fd;

    fd = open(file_name, O_RDONLY);
    if(fd == -1)
    {
        // write(2, "my_Shell: ", 10);
        // write(2, file_name, ft_strlen(file_name));
        // write(2, ": No such file or directory\n", 28);
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

void handle_herdoc(t_cmd cmd)
{
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
            int tmp_in = dup(0);
            dup2(cmd.old_out, 1);
            dup2(cmd.old_in,0);
            path = ft_strjoin("/tmp/", cur->file);  
            unlink(path);
            fd = open(path, O_CREAT | O_RDWR | O_TRUNC, 0644);
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
            free(path);
            dup2(tmp,1);
            dup2(tmp_in,0);
        }
        cur = cur->next;
    }
        dup2(fd, 0);
        close(fd);
}

void handle_redirection(t_cmd cmd)
{
    t_rd *curr;

    curr = cmd.rd;
    while (curr)
    {
        if (curr->type == R_OUT) // >
            exucute_red_out(curr->file);
        else if (curr->type == DR_OUT) // >>
            exucute_double_red_out(curr->file);
        else if (curr->type == R_IN) // >
            exucute_red_in(curr->file);
        curr = curr->next;
    }
}
