/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parss_red.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 21:53:57 by bgannoun          #+#    #+#             */
/*   Updated: 2023/07/21 21:58:38 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_rd	*creat_red(t_rd *head, char *file, int type)
{
	t_rd	*new_node;
	t_rd	*tmp;

	new_node = (t_rd *)malloc(sizeof(t_rd));
	new_node->file = ft_strdup(file);
	new_node->type = type;
	new_node->next = NULL;
	if (head == NULL)
		head = new_node;
	else
	{
		tmp = head;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new_node;
	}
	return (head);
}

void	prass_1(t_parss *glo, t_info **info)
{
	glo->tmp = *info;
	glo->global = malloc(sizeof(t_cmd) * nbr_cmd(*info));
	glo->global = cmd_init(glo->global, *info, nbr_cmd(*info));
	glo->indice = 0;
}

t_cmd	*parss_redirection(t_info **info)
{
	t_parss	g;

	prass_1(&g, info);
	while (g.tmp)
	{
		if ((g.tmp->type == R_OUT || g.tmp->type == R_IN || g.tmp->type == DR_IN
				|| g.tmp->type == DR_OUT) && g.tmp->next)
		{
			g.next_node = g.tmp->next;
			g.next_next_node = g.tmp->next->next;
			g.global[g.indice].rd = creat_red(g.global[g.indice].rd,
					g.next_node->content, g.tmp->type);
			*info = delete_node(*info, g.next_node);
			*info = delete_node(*info, g.tmp);
			g.tmp = g.next_next_node;
		}
		else if (g.tmp->type == PIPE)
		{
			g.indice++;
			g.tmp = g.tmp->next;
		}
		else
			g.tmp = g.tmp->next;
	}
	return (g.global);
}
