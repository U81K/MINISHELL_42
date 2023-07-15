/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 10:26:45 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/15 13:19:24 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_info	*remove_quots(t_info *info)
{
	t_info	*curr;
	t_info	*nex_node;

	curr = info;
	while (curr)
	{
		if (curr->state == NORMAL && (curr->type == QUOT || curr->type == D_QUOT))
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

int	check_quoting(t_info *info)
{
	int		s_counter;
	int		d_counter;
	t_info	*tmp;

	s_counter = 0;
	d_counter = 0;
	tmp = info;
	while (tmp)
	{
		if (tmp->type == QUOT && tmp->state == NORMAL)
			s_counter++;
		else if (tmp->type == D_QUOT && tmp->state == NORMAL)
			d_counter++;
		tmp = tmp->next;
	}
	if (s_counter % 2 == 0 && d_counter % 2 == 0)
		return (1);
	else
	{
		write(2, "my_Shell-310$: enclosed quotes\n", 32);
		return (0);
	}
	exist_status = 2;
	return (0);
}

int	cheak_pipes(t_info *info)
{
	t_info	*tmp;

	tmp = info;
	if (tmp->type == PIPE)
	{
		write(2, "my_Shell: syntax error near unexpected token `", 46);
		write(2, tmp->content, ft_strlen(tmp->content));
		write(2, "\'\n", 2);
		return (0);
	}
	while (tmp)
	{
		if (tmp->type == PIPE && (!tmp->next || tmp->next->type == PIPE))
		{
			write(2, "my_Shell: syntax error near unexpected token `", 46);
			write(2, tmp->content, ft_strlen(tmp->content));
			write(2, "\'\n", 2);
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

int	cheack_red(t_info *info)
{
	t_info	*tmp;

	tmp = info;
	while (tmp)
	{
		if ((tmp->type == R_OUT || tmp->type == R_IN || tmp->type == DR_IN
				|| tmp->type == DR_OUT))
		{
			if (tmp->next && tmp->next->type != WORD)
			{
				write(2, "my_Shell: syntax error near unexpected token `", 46);
				write(2, tmp->next->content, ft_strlen(tmp->next->content));
				write(2, "\'\n", 2);
				return (0);
			}
			else if (!tmp->next)
			{
				write(2, "my_Shell: syntax error near unexpected token `newline'\n", 55);
				return (0);
			}
		}
		tmp = tmp->next;
	}
	return (1);
}

int	cheack_syntax(t_info *info)
{
	if(!info)
	{
		// write(2, "my_Shell: : command not found\n", 30);
		return 0;
	}
	if (!cheak_pipes(info) || !cheack_red(info))
		return (exist_status = 2, 0);
	return (1);
}
