/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_tools.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 19:38:22 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/15 19:39:36 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_info *process_exit_status(t_info *info)
{
    t_info *tmp = info;
    while (tmp)
    {
        if (tmp->type == EXIT_S && tmp->state != IN_QUOT)
        {
            free(tmp->content);
            tmp->content = ft_itoa(exist_status);
            tmp->type = WORD;
            tmp = tmp->next;
        }
        else
            tmp = tmp->next;
    }
    return info;
}


t_info *process_dollar_after_expand(t_info *info)
{
    t_info *tmp = info;
    while (tmp)
    {
        if (tmp->type == VAR && (tmp->state == NORMAL || tmp->state == IN_D_QUOT))
        {
            if (tmp->content[0] == '$' && ft_strlen(tmp->content) == 1)
                tmp->type = WORD;
        }
        tmp = tmp->next;
    }
    return info;
}
