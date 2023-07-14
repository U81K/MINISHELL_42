/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 10:59:39 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/14 16:18:17 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int main()
{
    char *input;
    extern char **environ;
    t_env *env = NULL;

    env = ft_env(environ);
    while(1)
    {
        t_info *info = NULL;
        t_cmd *cmd = NULL;
        input = readline("\e[1;32mmy_Shell-310$ \e[0m");
        if(!input)
        {
            printf("\n");
            break;
        }
        if(input[0] == '\0')
        {
            free(input);
            continue;
        }
        if(ft_strlen(input) > 0)
            add_history(input);
        info = lexer(info, input);
        if(!check_quoting(info))
        {
            free_list(info);
            continue;
        }
        info = remove_quots(info);
        // print_list(info);
        info = remove_space_and_expand(info, env);
        if(!cheack_syntax(info))
        {
            free_list(info);
            continue;
        }
        cmd = parss_redirection(info); // sgft
        cmd = get_cmd_and_args(cmd, info);
        env = commands(cmd, env, info);
        free_list_cmd(cmd, info);
        free_list(info);
    }
    return 0;
}
