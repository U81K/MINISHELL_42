/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 17:27:40 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/27 01:34:13 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_env(t_env *head, t_cmd *cmd)
{
	t_env	*tmp;

	handle_redirection(*cmd);
	tmp = head;
	while (tmp && tmp->value)
	{
		write(1, tmp->key, ft_strlen(tmp->key));
		write(1, "=", 1);
		write(1, tmp->value, ft_strlen(tmp->value));
		write(1, "\n", 1);
		tmp = tmp->next;
	}
	dup2(cmd->old_out, 1);
	close(cmd->old_out);
	g_exit_status = 0;
}
