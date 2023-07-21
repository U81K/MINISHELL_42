/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 20:09:02 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/15 20:11:08 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid(char *str)
{
	int	j;

	j = 0;
	if ((ft_isalpha(str[j]) || str[j] == '_'))
		j++;
	else
		return (0);
	while (str[j] && str[j] != '=')
	{
		if (!ft_isalnum(str[j]) && str[j] != '_')
			return (0);
		j++;
	}
	return (1);
}

t_env	*search_list(t_env *head, char *str)
{
	t_env	*tmp;
	char	*key;
	char	*value;

	tmp = head;
	key = set_variables(str);
	value = set_value(str);
	while (tmp)
	{
		if (compare(tmp->key, key) && value)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			free(value);
			free(key);
			return (head);
		}
		tmp = tmp->next;
	}
	free(value);
	free(key);
	return (head);
}

t_env	*add_to_env(t_env *head, char *key, char *value, int p)
{
	t_env	*new_node;
	t_env	*tmp;

	new_node = (t_env *)malloc(sizeof(t_env));
	new_node->key = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
	else if (p)
		new_node->value = ft_strdup("");
	else
		new_node->value = NULL;
	free(key);
	free(value);
	new_node->next = NULL;
	if (head == NULL)
		head = new_node;
	else
	{
		tmp = head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
	return (head);
}

int	find_value_in_list(t_env *env, char *str)
{
	t_env	*tmp;
	char	*to_free;

	tmp = env;
	to_free = set_variables(str);
	while (tmp)
	{
		if (compare(tmp->key, to_free))
		{
			free(to_free);
			return (1);
		}
		tmp = tmp->next;
	}
	free(to_free);
	return (0);
}
