/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 17:33:43 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/13 17:34:14 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int find_value(t_env *head, char *str)
{
    t_env *curr;
    curr = head;

    while (curr)
    {
        if(!compare(str, curr->key))
            curr = curr->next;
        else
            return 1;
    }
    return 0;
}

t_env *unset_node(t_env *head, char *to_delete)
{
    t_env *curr;
    t_env *prev;

    curr = head;
    prev = head;

    if(!find_value(head, to_delete))
        return head;
    else
    {
        if(compare(to_delete, curr->key))
        {
            head = curr->next;
            free(curr->key);
            free(curr->value);
            free(curr);
            return head;
        }
        while (!compare(to_delete, curr->key))
        {
            prev = curr;
            curr = curr->next;
        }
        prev->next = curr->next;
        free(curr->key);
        free(curr->value);
        free(curr);
        return head;
    }
    return head;
}

t_env *unset(t_cmd cmd, t_env *env)
{
    int j = 1;
    while (j < cmd.nbr_arg)
    {
        if(is_valid(cmd.full_cmd[j]))
            env = unset_node(env, cmd.full_cmd[j]);
        else
        {
            write(2, "my_Shell: unset: ", 17);
            printf("`%s': not a valid identifier\n", cmd.full_cmd[j]);
        }
        j++;
    }
    return env;
}
