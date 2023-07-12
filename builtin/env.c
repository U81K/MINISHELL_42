/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 17:27:40 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/12 17:28:42 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void print_env(t_env *head)
{
    t_env *tmp;
    tmp = head;
    while (tmp && tmp->value[0] != '\0')
    {
        printf("%s=%s\n", tmp->key, tmp->value);
        tmp= tmp->next;
    }
}
