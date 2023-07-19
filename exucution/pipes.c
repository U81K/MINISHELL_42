/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 09:49:49 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/19 19:10:05 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// to cheack
void wait_for_child(int numb_of_cmd, int (*fd)[2], int *pid)
{
    int h;
    int i = 0;
    while(i < numb_of_cmd)
    {
        waitpid(pid[i], &h, 0);
        if(i == numb_of_cmd - 1)
            exist_status = WEXITSTATUS(h);
        i++;
    }
    free(pid);
    free(fd);
}

void creat_pipes(int numb_of_cmd, int (*fd)[2])
{
    int i = 0;
    while (i < numb_of_cmd)
    {
        pipe(fd[i]);
        i++;
    }
}

void close_fd(int num, int (*fd)[2])
{
    int i = 0;
    while (i < num)
    {
        close(fd[i][0]);
        close(fd[i][1]);
        i++;
    }
}

void redirect_fd_to_pipe_and_close(int num_of_cmd, int(*fd)[2], int index)
{
    if(index == 0)
    {
        dup2(fd[index][1], STDOUT_FILENO);
        close_fd(num_of_cmd, fd);
    }
    else if(index == num_of_cmd - 1)
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

t_env *run_commands(t_cmd *cmd, t_env *env, t_info *info) 
{
    int num_c;
    int *pid;
    int (*fd)[2];
    int i;

    num_c = nbr_cmd(info);
    if(num_c <= 1)
    {   
        cmd[0].old_in = dup(STDIN_FILENO);
        cmd[0].old_out = dup(STDOUT_FILENO);
        env = commands(&cmd[0], env, info);
    }
    else
    {
        pid = malloc(sizeof(int) * num_c);
        fd = malloc(sizeof(int) * num_c * 2);
        creat_pipes(num_c, fd);
        i = 0;
        while (i < num_c)
        {
            pid[i] = fork();
            if(pid[i] == 0)
            {
                cmd[i].old_in = dup(STDIN_FILENO);
                cmd[i].old_out = dup(STDOUT_FILENO);
                redirect_fd_to_pipe_and_close(num_c, fd, i);
                env = commands(&cmd[i], env, info);
                exit(exist_status);
            }
            i++;
        }
        close_fd(num_c, fd);
        wait_for_child(num_c, fd, pid);
    }
    return env;
}
