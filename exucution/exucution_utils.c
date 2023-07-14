/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exucution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 16:05:00 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/14 20:03:50 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int compare_until(char *s1, char *s2, int n)
{
    int i;
    i = 0;
    while (i < n)
    {
        if (s1[i] == s2[i])
            i++;
        else
            return 0;
    }
    return 1;
}

char *find_path(char **env, int j, char *str)
{   
    char *paths = NULL;
    int i = 0;

    if(!env)
        return NULL;

    while (env[i])
    {
        if(compare_until(env[i], str, j))
        {
            paths = malloc(sizeof(char) * (ft_strlen(env[i]) + 1 - ft_strlen(str)));
            copy_str(paths, env[i] + 5);
            break;
        }
        i++;
    }
    return paths;
}

void free_tab(char **tab)
{
    int i = 0;
    while (tab[i])
        free(tab[i++]);
    free(tab);
}

int nbr_of_element(t_env *env)
{
    int counter = 0;
    t_env *tmp = env;
    while (tmp)
    {
        counter ++;
        tmp = tmp->next;
    }
    return counter;
}

char **from_list_to_tab(t_env *env)
{
    t_env *tmp;
    char **tab;
    int i = 0;

    tab = malloc(sizeof(char *) * (nbr_of_element(env) + 1));
    tmp = env;
    i = 0;
    while (tmp)
    {
        tab[i] = malloc(sizeof(char) * (ft_strlen(tmp->key) + ft_strlen(tmp->value) + 2));
        copy_str(tab[i], tmp->key);
        tab[i][ft_strlen(tmp->key)] = '=';
        copy_str(tab[i] + ft_strlen(tmp->key) + 1, tmp->value);
        tmp = tmp->next;
        i++;
    }
    tab[i] = NULL;
    return tab;
}
