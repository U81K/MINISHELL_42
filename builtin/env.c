/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 17:27:40 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/21 13:13:52 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void print_env(t_env *head)
// {
//     t_env *tmp;
//     tmp = head;
//     while (tmp && tmp->value[0] != '\0')
//     {
//         printf("%s=%s\n", tmp->key, tmp->value);
//         tmp= tmp->next;
//     }
//     exist_status = 0;
// }

void print_env(t_env *head)
{
    t_env *tmp;
    tmp = head;
    while (tmp && tmp->value)
    {
        write(1, tmp->key, ft_strlen(tmp->key));
        write(1, "=", 1);
        write(1, tmp->value, ft_strlen(tmp->value));
        write(1, "\n", 1);
        // printf("%s=%s\n", tmp->key, tmp->value);
        tmp= tmp->next;
    }
    exist_status = 0;
}
