/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parssing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 10:05:20 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/15 12:16:35 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void joind_and_free_next(t_info *node, t_state state, t_info *to_delete)
{
    char *tmp_str;
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
			while (tmp->next && tmp->next->type == WORD) // ???
                joind_and_free_next(tmp, tmp->state, tmp->next);
		if (tmp->state == IN_QUOT)
			while (tmp->next && tmp->next->state == IN_QUOT)
                joind_and_free_next(tmp, IN_QUOT, tmp->next);
		if (tmp->type != VAR && tmp->type != EXIT_S && tmp->state == IN_D_QUOT)
		{
			while (tmp->next && tmp->next->type != VAR && tmp->next->type != EXIT_S && tmp->next->state == IN_D_QUOT)
                joind_and_free_next(tmp, tmp->state, tmp->next);
		}
		tmp = tmp->next;
	}
	return (info);
}

t_info *expand_var(t_env *env, t_info *info)
{
	t_env	*tmp_env;
	t_info	*tmp = info;
    tmp_env = env;

    while (tmp_env)
    {
        if (compare(tmp_env->key, tmp->content + 1))
        {
			if(exist_or_not(tmp_env->value, ' '))
			{
				char **tmp_tab = ft_split(tmp_env->value, ' ');
				int i = 0;
				while (tmp_tab[i])
				{
					info = creat_node(info, tmp_tab[i], WORD, NORMAL);
					info = creat_node(info, " ", S_SPACE, NORMAL);
					i++;
				}
			}
			else
			{
				free(tmp->content);
				tmp->content = ft_strdup(tmp_env->value);
				tmp->state = NORMAL;
				tmp->type = WORD;
			}
        }
        tmp_env = tmp_env->next;
    }
    return info;
}

t_info	*expand_variable(t_info *info, t_env *env)
{
	t_info	*tmp;
	t_info	*nex_node;

	tmp = info;
	while (tmp)
	{
		if (tmp->type == VAR && (tmp->state == NORMAL || tmp->state == IN_D_QUOT))
		{
            tmp = expand_var(env, tmp);
			if(tmp->content[0] == '$' && ft_strlen(tmp->content) == 1)
			{
				tmp->type = WORD;
				tmp = tmp->next;
			}
			else if (tmp->content[0] == '$' && ft_strlen(tmp->content) != 1)
			{
				nex_node = tmp->next;
				info = delete_node(info, tmp);
				tmp = nex_node;
				continue;
			}
		}
		else if(tmp->type == EXIT_S && tmp->state != IN_QUOT)
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

t_rd	*creat_redirection(t_rd *head, char *file, int type)
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

t_cmd	*parss_redirection(t_info *info)
{
	int		cmd_nbr;
	int		indice;

	t_cmd	*global;

	t_info	*tmp;
	t_info	*next_node;
	t_info	*next_next_node;

	tmp = info;
	cmd_nbr = nbr_cmd(info);
	global = malloc(sizeof(t_cmd) * cmd_nbr);
	global = cmd_init(global, info, cmd_nbr);

	indice = 0;
	while (tmp)
	{
		if ((tmp->type == R_OUT || tmp->type == R_IN || tmp->type == DR_IN || tmp->type == DR_OUT) && tmp->next)
		{
			next_node = tmp->next;
			next_next_node = tmp->next->next;
			global[indice].rd = creat_redirection(global[indice].rd, next_node->content, tmp->type);
			info = delete_node(info, next_node);
			info = delete_node(info, tmp);
			tmp = next_next_node;
		}
		else if (tmp->type == PIPE)
		{
			indice++;
			tmp = tmp->next;
		}
		else
			tmp = tmp->next;
	}
	return (global);
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
		while (j < cmd[i].nbr_arg)
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
