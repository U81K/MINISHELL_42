/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 10:00:09 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/21 20:51:23 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_info	*handle_output_redirection(t_info *info, char *line, int *i)
{
	if (line[*i + 1] == '>')
	{
		info = creat_node(info, ft_strdup(">>"), DR_OUT, NORMAL);
		*i += 2;
	}
	else
	{
		info = creat_node(info, ft_strdup(">"), R_OUT, NORMAL);
		(*i)++;
	}
	return (info);
}

t_info	*handle_inpute_redirection(t_info *info, char *line, int *i)
{
	if (line[*i + 1] == '<')
	{
		info = creat_node(info, ft_strdup("<<"), DR_IN, NORMAL);
		*i += 2;
	}
	else
	{
		info = creat_node(info, ft_strdup("<"), R_IN, NORMAL);
		(*i)++;
	}
	return (info);
}

t_info	*handle_quote(t_info *info, char *line, int *i)
{
	(void)line;
	info = creat_node(info, ft_strdup("\'"), QUOT, NORMAL);
	(*i)++;
	return (info);
}

t_info	*handle_double_quote(t_info *info, char *line, int *i)
{
	(void)line;
	info = creat_node(info, ft_strdup("\""), D_QUOT, NORMAL);
	(*i)++;
	return (info);
}

t_info	*set_state(t_info *info)
{
	t_info	*tmp;

	tmp = info;
	while (tmp)
	{
		if (tmp->type == D_QUOT && tmp->next)
			tmp = set_state_1(&tmp);
		else if (tmp->type == QUOT && tmp->next)
		{
			tmp = tmp->next;
			while (tmp->type != QUOT && tmp->next)
			{
				tmp->state = IN_QUOT;
				tmp->type = WORD;
				tmp = tmp->next;
			}
		}
		tmp = tmp->next;
	}
	return (info);
}
