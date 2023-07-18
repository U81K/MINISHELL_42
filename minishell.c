/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 10:59:39 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/18 21:56:42 by ybourais         ###   ########.fr       */
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
			info = delete_node(info, curr);
			curr = nex_node;
		}
		else
			curr = curr->next;
	}
	return (info);
}

t_info* copy_linked_list(const t_info* head)
{
    // if (head == NULL)
    //     return NULL;

    t_info* new_head = NULL;
    t_info* current = NULL;
    t_info* new_node = NULL;

    // Iterate through the original list
    for (const t_info* original_node = head; original_node != NULL; original_node = original_node->next)
    {
        // Create a new node
        new_node = malloc(sizeof(t_info));
        if (new_node == NULL)
        {
            // Handle memory allocation error
            // You can add error handling code or free the already allocated nodes before returning
            return NULL;
        }

        // Copy the content
        new_node->content = strdup(original_node->content);  // Assuming content is a dynamically allocated string

        // Copy other variables
        new_node->type = original_node->type;
        new_node->state = original_node->state;
        new_node->next = NULL;

        // Update the linked list
        if (new_head == NULL)
        {
            new_head = new_node;
            current = new_node;
        }
        else
        {
            current->next = new_node;
            current = new_node;
        }
    }

    return new_head;
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
        // print_list(info);
        // while(1);
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
