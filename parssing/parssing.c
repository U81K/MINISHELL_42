/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parssing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 10:05:20 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/21 21:54:23 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	joind_and_free_next(t_info *node, t_state state, t_info *to_delete)
{
	char	*tmp_str;

	tmp_str = ft_strjoin(node->content, node->next->content);
	free(node->content);
	node->content = ft_strdup(tmp_str);
	free(tmp_str);
	node->state = state;
	node->type = WORD;
	node->next = to_delete->next;
	free(to_delete->content);
	free(to_delete);
}

t_info	*join_content(t_info *info)
{
	t_info	*tmp;

	tmp = info;
	while (tmp && tmp->next)
	{
		if (tmp->type == WORD)
			while (tmp->next && tmp->next->type == WORD)
				joind_and_free_next(tmp, tmp->state, tmp->next);
		if (tmp->state == IN_QUOT)
			while (tmp->next && tmp->next->state == IN_QUOT)
				joind_and_free_next(tmp, IN_QUOT, tmp->next);
		if (tmp->type != VAR && tmp->type != EXIT_S && tmp->state == IN_D_QUOT)
		{
			while (tmp->next && tmp->next->type != VAR
				&& tmp->next->type != EXIT_S && tmp->next->state == IN_D_QUOT)
				joind_and_free_next(tmp, tmp->state, tmp->next);
		}
		tmp = tmp->next;
	}
	return (info);
}

t_cmd	*get_cmd_and_args(t_cmd *cmd, t_info *info)
{
	int	num;
	int	i;
	int	j;

	cmd = nbr_arg(info, cmd);
	num = nbr_cmd(info);
	i = 0;
	while (i < num)
	{
		cmd[i].full_cmd = malloc(sizeof(char *) * (cmd[i].nbr_arg + 1));
		j = 0;
		while (j < cmd[i].nbr_arg && info)
		{
			if (info->type != PIPE)
			{
				cmd[i].full_cmd[j] = ft_strdup(info->content);
				j++;
			}
			info = info->next;
		}
		cmd[i].full_cmd[j] = NULL;
		i++;
	}
	return (cmd);
}
