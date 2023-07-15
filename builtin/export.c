/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 17:16:43 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/15 20:14:16 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void print_export(t_env *env)
{
    t_env *tmp;
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

void print_export_error(char *str)
{
    write(2, "my Shell: export: `", 19);
    write(2, str, ft_strlen(str));
    write(2, "': not a valid identifier\n", 26);
    exist_status = 1;
}

t_env *every_thing_all_at_once(t_env *env, char *str, int *equal)
{
    if(exist_or_not(str, '='))
        *equal = 1;
    env = search_list(env, str);
    if(!find_value_in_list(env, str))
        env = add_to_env(env, set_variables(str), set_value(str), *equal);
    exist_status = 0;
    return env;
}

t_env *ft_export(t_cmd cmd, t_env *env)
{
    int equal;
    int i;

    i = 0;
    equal = 0;
    if (cmd.full_cmd[0] && !cmd.full_cmd[1])
        print_export(env);
    else
    {
        i = 1;
        while (i < cmd.nbr_arg)
        {
            if(is_valid(cmd.full_cmd[i]))
                env = every_thing_all_at_once(env, cmd.full_cmd[i], &equal);
            else
                print_export_error(cmd.full_cmd[i]);
            equal = 0;
            i++;
        }
    }
    return env;
}
