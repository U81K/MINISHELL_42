/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 12:03:03 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/13 12:04:02 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env *change_env(t_env *head, char *new, char *old)
{
    t_env *tmp;
    tmp = head;

    while (tmp)
    {
        if (compare(tmp->key, old))
        {
            free(tmp->value);
            tmp->value = ft_strdup(new);
        }
        tmp = tmp->next;
    }
    return head;
}

void cd(char **tab, t_env *head) 
{

    char cwd[1024];
    if(tab[0] && !tab[1])
    {
        head = change_env(head, getcwd(cwd, sizeof(cwd)), "OLDPWD");
        chdir(getenv("HOME"));
        head = change_env(head, getcwd(cwd, sizeof(cwd)), "PWD");
    }
    else if(tab[0] && tab[1] && !tab[2])
    {
        head = change_env(head, getcwd(cwd, sizeof(cwd)), "OLDPWD");
        chdir(tab[1]);
        head = change_env(head, getcwd(cwd, sizeof(cwd)), "PWD");
    }
    else 
        perror("my_shell");
}