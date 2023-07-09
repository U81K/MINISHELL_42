/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 11:44:34 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/09 11:56:04 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_space(char c)
{
    if(c == ' ' || c == '\t' || c == '\v')
        return 1;
    return 0;
}

int special_char(char c)
{
    if(is_space(c) || c == '\t' || c == '$' || c == '|' || c == '\'' || c == '\"' || c == '>' || c == '<' || c =='\0')
        return 1;
    return 0;
}

t_node *creat_env(t_node *head, char **env)
{
    int i = 0;
    while (env[i])
        head = creat_list(head, env[i++]);
    return head;
}

t_info *creat_node(t_info *head, char *content, t_type type, t_state state)
{
    t_info *new_node = (t_info*)malloc(sizeof(t_info));
    new_node->content = (char *)malloc(sizeof(char) * slen(content) + 1);
    copy_str(new_node->content, content);
    new_node->state = state;
    new_node->type = type;
    new_node->next = NULL;
    if(head == NULL)
        head = new_node;
    else
    {
        t_info *tmp = head;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = new_node;
    }
    return head;
}

t_info *lexer(t_info *info, char *line)
{
    int i = 0;
    int counter = 0;
    while (line[i])
    {
        if(line[i] == '$')
        {
            if(line[i + 1] == '?')
            {
                info = creat_node(info, ft_strdup("$?"), EXIT_S, NORMAL);
                i+=2;
                continue;
            }
            else if(ft_isdigit(line[i + 1]))
            {
                info = creat_node(info, ft_substr(line, i, i + 1), VAR, NORMAL);
                i+=2;
                continue;
            }
            while (!special_char(line[i + 1]))
                counter++;
            info = creat_node(info, ft_substr(line, i, counter), VAR, NORMAL);
            i = i + counter;
            counter = 0;
        }
        else if(is_space(line[i]))
        {
            info = creat_node(info, ft_strdup(" "), S_SPACE, NORMAL);
            i++;
        }
        else if(!special_char(line[i]))
        {
            while (!special_char(line[i + 1]))
                counter++;
            info = creat_node(info, ft_substr(line, i, counter), WORD, NORMAL);
            i = i + counter;
            counter = 0;
        }
        else if(line[i] == '>')
        {
            if(line[i + 1] == '>')
            {
                info = creat_node(info, ft_strdup(">>"), DR_OUT, NORMAL);
                i+=2;
            }
            else
            {
                info = creat_node(info, ft_strdup(">"), R_OUT, NORMAL);
                i++;
            }
        }
        else if(line[i] == '<')
        {
            if(line[i + 1] == '<')
            {
                info = creat_node(info, ft_strdup("<<"), DR_IN, NORMAL);
                i+=2;
            }
            else
            {
                info = creat_node(info, ft_strdup("<"), R_IN, NORMAL);
                i++;
            }
        }
        else if(line[i] == '|')
        {
            info = creat_node(info, ft_strdup("|"), PIPE, NORMAL);
            i++;
        }
    }
    return info;
}

void print_linked_list(t_info *head) {
    t_info *current = head;

    printf("|   Content   |   Type   |   State   |\n");
    printf("-------------------------------------\n");

    while (current != NULL) {
        printf("|  %-10s |    %c     |     %d     |\n",
               current->content, (char)current->type, current->state);
        current = current->next;
    }
}

void free_linked_list(t_info *head) {
    t_info *current = head;
    t_info *next;

    while (current != NULL) {
        next = current->next;
        free(current->content);
        free(current);
        current = next;
    }
}

int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;

    char *input;
    // char **tab;
    // int *arr = {0};
    t_node *head = NULL;
    t_info *info = NULL;

    head = creat_env(head, env);
    while(1)
    {
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
        add_history(input);
        info = lexer(info, input);
        print_linked_list(info);
        // free_linked_list(info);
        // arr = check_quoting(input);
        // tab = split(input, ' ');
        // head = commands(tab, head, arr);
        // free_tab(tab);
        // free(input);
    }
    return 0;
}
