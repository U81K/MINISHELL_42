/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_tools.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 19:38:22 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/21 22:38:35 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_info	*process_exit_status(t_info *info)
{
	t_info	*tmp;

	tmp = info;
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
	return (info);
}

t_info	*process_dollar_after_expand(t_info *info)
{
	t_info	*tmp;

	tmp = info;
	while (tmp)
	{
		if (tmp->type == VAR && (tmp->state == NORMAL
				|| tmp->state == IN_D_QUOT))
		{
			if (tmp->content[0] == '$' && ft_strlen(tmp->content) == 1)
				tmp->type = WORD;
		}
		tmp = tmp->next;
	}
	return (info);
}

t_info	*remove_space(t_info *info)
{
	t_info	*curr;
	t_info	*nex_node;

	curr = info;
	while (curr)
	{
		if (curr->state == NORMAL && curr->type == S_SPACE)
		{
			nex_node = curr->next;
			info = delete_node(info, curr);
			curr = nex_node;
		}
		else
			curr = curr->next;
	}
	return (info);
}