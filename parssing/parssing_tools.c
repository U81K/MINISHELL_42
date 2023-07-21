/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parssing_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 13:19:50 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/21 16:19:28 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	compare(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (0);
	if (ft_strlen(s1) != ft_strlen(s2))
		return (0);
	while (s1[i] && s1[i])
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}
	return (1);
}

int	nbr_cmd(t_info *info)
{
	t_info	*tmp;
	int		counter;

	tmp = info;
	counter = 0;
	while (tmp)
	{
		if (tmp->type == PIPE || !tmp->next)
			counter++;
		tmp = tmp->next;
	}
	return (counter);
}

t_cmd	*cmd_init(t_cmd *cmd, t_info *info, int nbr)
{
	int	i;

	(void)info;
	i = 0;
	while (i < nbr)
	{
		cmd[i].full_cmd = NULL;
		cmd[i].rd = NULL;
		cmd[i].nbr_arg = 0;
		cmd[i].nbr_cmd = nbr;
		i++;
	}
	return (cmd);
}

t_cmd	*nbr_arg(t_info *info, t_cmd *cmd)
{
	int	counter;
	int	i;

	counter = 0;
	i = 0;
	while (info)
	{
		if (info->type == PIPE || !info->next)
		{
			cmd[i].nbr_arg = counter;
			if (!info->next)
				cmd[i].nbr_arg = counter + 1;
			counter = -1;
			i++;
		}
		counter++;
		info = info->next;
	}
	return (cmd);
}

void	free_red(t_rd *head)
{
	t_rd	*current;
	t_rd	*next;

	current = head;
	if (!current)
		return ;
	while (current)
	{
		next = current->next;
		free(current->file);
		free(current);
		current = next;
	}
}
