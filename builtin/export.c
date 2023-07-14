/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 17:16:43 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/14 19:50:15 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int is_valid(char *str)
{
    int j = 0;

    if ((ft_isalpha(str[j]) || str[j] == '_'))
        j++;
    else
        return 0;
    while (str[j] && str[j] != '=')
    {
        if (!ft_isalnum(str[j]) && str[j] != '_')
            return 0;
        j++;
    }
    return 1;
}

t_env *search_list(t_env *head, char *str)
{
    t_env *tmp;
    tmp = head;

    char *key = set_variables(str);
    char *value = set_value(str);

    while (tmp)
    {
        if (compare(tmp->key, key) && value)
        {
            free(tmp->value);
            tmp->value = ft_strdup(value);
            return head;
        }   
        tmp = tmp->next;
    }
    return head;
}

t_env	*add_to_env(t_env *head, char *key, char *value, int p)
{
	t_env	*new_node;
	t_env	*tmp;

	new_node = (t_env *)malloc(sizeof(t_env));
	new_node->key = ft_strdup(key);
    if(value)
        new_node->value = ft_strdup(value);
    else if(p)
        new_node->value = ft_strdup("");
    else
        new_node->value = NULL;
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

int find_value_in_list(t_env *env, char *str)
{
    t_env *tmp = env;
    while (tmp)
    {
        if(compare(tmp->key, set_variables(str)))
            return 1;
        tmp = tmp->next;
    }
    return 0;
}

t_env *ft_export(t_cmd cmd, t_env *env, t_info *info)
{
    t_env *tmp;
    (void)info;
    int i = 0;
    int equal = 0;

    if (cmd.full_cmd[0] && !cmd.full_cmd[1])
    {
        tmp = env;
        while (tmp)
        {
            printf("declare -x %s", tmp->key);
            if(tmp->value != NULL)
                printf("=\"%s\"\n", tmp->value);
            else
                printf("\n");
            tmp= tmp->next;
        }
        exist_status = 0;
    }
    else
    {
        i = 1;
        while (i < cmd.nbr_arg)
        {
            if(is_valid(cmd.full_cmd[i]))
            {
                if(exist_or_not(cmd.full_cmd[i], '='))
                    equal = 1;
                env = search_list(env, cmd.full_cmd[i]);
                if(!find_value_in_list(env, cmd.full_cmd[i]))
                    env = add_to_env(env, set_variables(cmd.full_cmd[i]), set_value(cmd.full_cmd[i]), equal);
                exist_status = 0;
            } 
            else
            {
                write(2, "my Shell: export: `", 19);
                write(2, cmd.full_cmd[i], ft_strlen(cmd.full_cmd[i]));
                write(2, "': not a valid identifier\n", 26);
                exist_status = 1;
            }
            equal = 0;
            i++;
        }
    }
    return env;
}
