/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 17:33:43 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/21 22:48:03 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_value(t_env *head, char *str)
{
	t_env	*curr;

	curr = head;
	while (curr)
	{
		if (!compare(str, curr->key))
			curr = curr->next;
		else
			return (1);
	}
	return (0);
}

void	free_for_unset(t_env *curr)
{
	free(curr->key);
	free(curr->value);
	free(curr);
}

t_env	*unset_node(t_env *head, char *to_delete)
{
	t_env	*curr;
	t_env	*prev;

	curr = head;
	prev = head;
	if (!find_value(head, to_delete))
		return (head);
	else
	{
		if (compare(to_delete, curr->key))
		{
			free_for_unset(head);
			head = curr->next;
			return (head);
		}
		while (!compare(to_delete, curr->key))
		{
			prev = curr;
			curr = curr->next;
		}
		prev->next = curr->next;
		free_for_unset(curr);
		return (head);
	}
	return (head);
}

t_env	*unset(t_cmd cmd, t_env *env)
{
	int	j;

	j = 1;
	while (j < cmd.nbr_arg)
	{
		if (is_valid(cmd.full_cmd[j]))
		{
			env = unset_node(env, cmd.full_cmd[j]);
			g_exit_status = 0;
		}
		else
		{
			write(2, "my Shell: unset: `", 18);
			write(2, cmd.full_cmd[j], ft_strlen(cmd.full_cmd[j]));
			write(2, "': not a valid identifier\n", 26);
			g_exit_status = 1;
		}
		j++;
	}
	return (env);
}
