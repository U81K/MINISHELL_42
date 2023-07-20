/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 10:59:39 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/20 11:28:22 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_info	*remove_space(t_info *info)
{
	t_info	*curr;
	t_info	*nex_node;

	curr = info;
	while (curr)
	{
		if (curr->state == NORMAL && curr->type == S_SPACE)
		{
			nex_node = curr->next;
            // while(1);
			info = delete_node(info, curr);
			curr = nex_node;
		}
		else
			curr = curr->next;
	}
	return (info);
}

int main()
{
    char *input;
    extern char **environ;
    t_env *env;

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
        info = join_content(info);
        info = remove_quots(info);
        info = expand_variable(info, env);
        info = join_content(info);
        info = remove_space(info);
        if(!cheack_syntax(info))
        {
            free_list(info);
            continue;
        }
        cmd = parss_redirection(&info);
        cmd = get_cmd_and_args(cmd, info);
        env = run_commands(cmd, env, info);
        free_list_cmd(cmd, info);
        free_list(info);
    }
    return 0;
}
