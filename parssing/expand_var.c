/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 19:33:31 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/15 19:39:31 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_info *if_space_is_there(char *str, t_info *info)
{
	char **tmp_tab;

	tmp_tab = ft_split(str, ' ');
	int i = 0;
	while (tmp_tab[i])
	{
		info = creat_node(info, tmp_tab[i], WORD, NORMAL);
		info = creat_node(info, " ", S_SPACE, NORMAL);
		i++;
	}
	free_tab(tmp_tab);
	return info;
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
				info = if_space_is_there(tmp_env->value, info);
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

t_info *expand_existing_var(t_info *info, t_env *env)
{
    t_info *tmp = info;
    while (tmp)
    {
        if (tmp->type == VAR && (tmp->state == NORMAL || tmp->state == IN_D_QUOT))
        {
            tmp = expand_var(env, tmp);
            if (tmp->content[0] == '$' && ft_strlen(tmp->content) != 1)
            {
                t_info *nex_node = tmp->next;
                info = delete_node(info, tmp);
                tmp = nex_node;
                continue;
            }
        }
        tmp = tmp->next;
    }
    return info;
}


t_info *expand_variable(t_info *info, t_env *env)
{
    info = expand_existing_var(info, env);
    info = process_dollar_after_expand(info);
    info = process_exit_status(info);
    return info;
}
