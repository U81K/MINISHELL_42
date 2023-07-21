/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 11:18:57 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/21 20:54:22 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_info	*delete_node(t_info *head, t_info *to_delete)
{
	t_info	*tmp;

	tmp = NULL;
	tmp = head;
	if (to_delete == head)
	{
		head = tmp->next;
		free(tmp->content);
		free(tmp);
	}
	else
	{
		while (tmp->next != to_delete)
			tmp = tmp->next;
		tmp->next = to_delete->next;
		free(to_delete->content);
		free(to_delete);
	}
	return (head);
}

t_info	*creat_node(t_info *head, char *content, t_type type, t_state state)
{
	t_info	*new_node;
	t_info	*tmp;

	new_node = (t_info *)malloc(sizeof(t_info));
	new_node->content = content;
	new_node->state = state;
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

t_env	*creat_liste(t_env *head, char *env)
{
	t_env	*new_node;
	t_env	*tmp;

	new_node = malloc(sizeof(t_env));
	new_node->key = set_variables(env);
	new_node->value = set_value(env);
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

t_env	*ft_env(char **tab)
{
	int		i;
	t_env	*head;

	i = 0;
	head = NULL;
	while (tab[i])
		head = creat_liste(head, tab[i++]);
	return (head);
}

void	free_list_cmd(t_cmd *head, t_info *info)
{
	int	i;
	int	num;

	(void)info;
	num = head->nbr_cmd;
	i = 0;
	while (i < num)
	{
		free_tab(head[i].full_cmd);
		free_red(head[i].rd);
		i++;
	}
	free(head);
}

// void	free_list_cmd(t_cmd *head, t_info *info)
// {
// 	int	i;

// 	i = nbr_cmd(info) - 1;
// 	while (i >= 0)
// 	{
// 		free_tab(head[i].full_cmd);
// 		free_red(head[i].rd);
// 		free(&head[i]);
// 		i--;
// 	}
// }
