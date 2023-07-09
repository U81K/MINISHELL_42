/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 13:13:58 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/05 15:58:05 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node *creat_list(t_node *head, char *env)
{
    t_node *new_node = malloc(sizeof(t_node));
    new_node->var = malloc(sizeof(char) * slen(env) + 1);
    copy_str(new_node->var, env);
    new_node->next = NULL;
    if(head == NULL)
        head = new_node;
    else
    {
        t_node *tmp = head;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = new_node;
    }
    return head;
}

t_node *search_list(t_node *head, char *str, int *p)
{
    t_node *tmp;
    tmp = head;

    while (tmp)
    {
        if (compare_until(tmp->var, str, strchrch(tmp->var, '=')))
        {
            head = replace_node(head, str, tmp->var);
            *p = 1;
            return head;
        }
        tmp = tmp->next;
    }
    *p = 0;
    return head;
}

t_node *replace_node(t_node *head, char *new, char *to_delete)
{
    t_node *new_node;
    new_node = malloc(sizeof(t_node));
    new_node->var = malloc(sizeof(char) * slen(new) + 1);
    copy_str(new_node->var, new);

    t_node *curr;
    curr = head;
    if(compare(curr->var, to_delete))
    {
        new_node->next = head->next;
        head = new_node;
        free(curr->var);
        free(curr);
        return head;
    }
    t_node *prev;
    curr = head;
    prev = head;
    while (!compare(curr->var, to_delete))
    {
        prev = curr;
        curr = curr->next;
    }
    prev->next = new_node;
    new_node->next = curr->next;
    free(curr->var);
    free(curr);
    return head;
}

t_node *unset_node(t_node *head, char *to_delete)
{
    t_node *curr;
    t_node *prev;

    curr = head;
    prev = head;

    if(!find_value(head, to_delete))
        return head;
    else
    {
        if(compare_len(to_delete, curr->var))
        {
            head = curr->next;
            free(curr->var);
            free(curr);
            return head;
        }
        while (!compare_len(to_delete, curr->var))
        {
            prev = curr;
            curr = curr->next;
        }
        prev->next = curr->next;
        free(curr->var);
        free(curr);
        return head;
    }
    return head;
}

int find_value(t_node *head, char *str)
{
    t_node *curr;
    t_node *after;

    curr = head;
    after = head;

    while (curr)
    {
        if(!compare_len(str, curr->var))
        {
            after = curr;
            curr = after->next;
        }
        else
            return 1;
    }
    return 0;
}
