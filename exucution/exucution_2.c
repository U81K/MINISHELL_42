/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exucution_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 13:34:06 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/18 15:01:43 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


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


void to_write_error(char *cmd, char *msg, int len, int exit_status)
{
    write(2, cmd, ft_strlen(cmd));
    write(2, msg, len);
    exist_status = exit_status;
}



void free_and_wait(t_tool *tool)
{
    int h;
    waitpid(tool->pid, &h, 0);
    exist_status = WEXITSTATUS(h);
    free_tab(tool->env);
    free_tab(tool->paths);
    free(tool->path);
    free(tool);
}
