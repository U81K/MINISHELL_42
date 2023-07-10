/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 11:44:34 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/10 11:22:57 by ybourais         ###   ########.fr       */
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
    if(is_space(c) || c == '\t' || c == '$' || c == '|' || c == '\'' || c == '\"' || c == '>' || c == '<' || c == '\0')
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
                info = creat_node(info, ft_substr(line, i, 2), VAR, NORMAL);
                i+=2;
                continue;
            }
            counter = 0;
            while (ft_isalnum(line[counter + i + 1]) || line[counter + i + 1] == '_') /* 1 added to cheak after the $ */
                counter++;
            info = creat_node(info, ft_substr(line, i, counter + 1), VAR, NORMAL); /* added 1 to capture the $ in the beginning */
            i = i + counter + 1; /*added one to skip the one char */
        }
        else if(is_space(line[i]))
        {
            info = creat_node(info, ft_strdup(" "), S_SPACE, NORMAL);
            i++;
        }
        else if(!special_char(line[i]))
        {
            counter = 0;
            while (!special_char(line[i + counter]))
                counter++;
            info = creat_node(info, ft_substr(line, i, counter), WORD, NORMAL);
            i = i + counter;
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
        else if(line[i] == '\'')
        {
            info = creat_node(info, ft_strdup("\'"), QUOT, NORMAL);
            i++;
        }
        else if(line[i] == '\"')
        {
            info = creat_node(info, ft_strdup("\""), D_QUOT, NORMAL);
            i++;
        }
    }
    return info;
}

char *get_type(t_type type) 
{
    if (type == QUOT)
        return "QUOT";
    else if (type == D_QUOT)
        return "D_QUOT";
    else if (type == S_SPACE)
        return "S_SPACE";
    else if (type == VAR)
        return "VAR";
    else if (type == PIPE)
        return "PIPE";
    else if (type == R_OUT)
        return "R_OUT";
    else if (type == R_IN)
        return "R_IN";
    else if (type == EXIT_S)
        return "EXIT_S";
    else if (type == WORD)
        return "WORD";
    else if (type == DR_IN)
        return "DR_IN";
    else if (type == DR_OUT)
        return "DR_OUT";
    return NULL;
}

char *get_state(t_state state) 
{
    if (state == IN_D_QUOT)
        return "IN_D_QUOT";
    else if (state == IN_QUOT)
        return "IN_QUOT";
    else if (state == NORMAL)
        return "NORMAL";
    return NULL;
}

void print_list(t_info *head) 
{
    t_info *current = head;

    printf("|   Content   |    Type    |   State   |\n");
    printf("---------------------------------------\n");

    while (current != NULL) 
    {
        printf("|  %-10s |  %-8s  |  %-8s |\n", current->content, get_type(current->type), get_state(current->state));
        current = current->next;
    }
}

void free_list(t_info *head) 
{
    t_info *current = head;
    t_info *next;

    while (current) 
    {
        next = current->next;
        free(current->content);
        free(current);
        current = next;
    }
}

t_info *set_state(t_info *info)
{
    t_info *tmp = info;

    while (tmp)
    {
        if (tmp->type == D_QUOT && tmp->next)
        {
            tmp = tmp->next;
            while (tmp->type != D_QUOT && tmp->next)
            {
                tmp->state = IN_D_QUOT;
                tmp = tmp->next;
            }
        }
        else if (tmp->type == QUOT && tmp->next)
        {
            tmp = tmp->next;
            while (tmp->type != QUOT && tmp->next)
            {
                tmp->state = IN_QUOT;
                tmp = tmp->next;
            }
        }
        tmp = tmp->next;
    }
    return info;
}

int cheak_quoting(t_info *info)
{
    int s_counter = 0;
    int d_counter = 0;
    t_info *tmp = info;

    while (tmp)
    {
        if(tmp->type == QUOT && tmp->state == NORMAL)
            s_counter++;
        else if (tmp->type == D_QUOT && tmp->state == NORMAL)
            d_counter++;
        tmp = tmp->next;
    }
    if(s_counter % 2 == 0 && d_counter % 2 == 0)
        return 1;
    else
    {
        write(2, "my_Shell-310$: enclosed quotes\n", 32);
        return 0;
    }
    return 0;
}

t_info* delete_node(t_info* head, t_info* to_delete) 
{
    t_info *tmp;
    tmp = head;
    if (to_delete == head)
    {
        head = tmp->next;
        free(tmp->content);
        free(tmp);
    }
    else
    {
        while (tmp->next != to_delete)
        {
            tmp = tmp->next;
        }
        tmp->next = to_delete->next;
        free(to_delete->content);
        free(to_delete);
    }
    return head;
}

t_info *remove_quots(t_info *info)
{
    t_info *curr;

    curr = info;

    while (curr)
    {
        if (curr->state == NORMAL && (curr->type == QUOT || curr->type == D_QUOT))
            info = delete_node(info, curr);
        curr = curr->next;
    }
    return info;
}
// t_info* recreate_linked_list(t_info* head) {
//     if (head == NULL) {
//         return NULL;
//     }

//     t_info* current = head;
//     t_info* new_head = NULL;
//     t_info* new_current = NULL;
//     char* joined_content = NULL;

//     while (current != NULL) {
//         if (current->state == NORMAL) {
//             // Create a new node and copy the content
//             t_info* new_node = (t_info*)malloc(sizeof(t_info));
//             new_node->content = strdup(current->content);
//             new_node->type = current->type;
//             new_node->state = current->state;
//             new_node->next = NULL;

//             if (new_head == NULL) {
//                 new_head = new_node;
//                 new_current = new_node;
//             } else {
//                 new_current->next = new_node;
//                 new_current = new_current->next;
//             }

//             joined_content = NULL;  // Reset joined_content for next IN_D_QUOT state
//         } else if (current->state == IN_QUOT) {
//             // Join the content with the next nodes until state changes
//             char* joined_content_quot = strdup(current->content);
//             t_info* next_node = current->next;

//             while (next_node != NULL && next_node->state == IN_QUOT) {
//                 joined_content_quot = realloc(joined_content_quot, strlen(joined_content_quot) + strlen(next_node->content) + 1);
//                 strcat(joined_content_quot, next_node->content);
//                 t_info* temp = next_node;
//                 next_node = next_node->next;
//                 free(temp->content);
//                 free(temp);
//             }

//             // Create a new node for the joined content
//             t_info* new_node = (t_info*)malloc(sizeof(t_info));
//             new_node->content = joined_content_quot;
//             new_node->type = current->type;
//             new_node->state = current->state;
//             new_node->next = NULL;

//             if (new_head == NULL) {
//                 new_head = new_node;
//                 new_current = new_node;
//             } else {
//                 new_current->next = new_node;
//                 new_current = new_current->next;
//             }

//             // Move to the next node after joining the content
//             current = next_node;
//             continue;
//         } else if (current->state == IN_D_QUOT) {
//             if (current->type == VAR) {
//                 // Create a new node for VAR node in IN_QUOT state
//                 t_info* new_node = (t_info*)malloc(sizeof(t_info));
//                 new_node->content = strdup(current->content);
//                 new_node->type = current->type;
//                 new_node->state = IN_QUOT;
//                 new_node->next = NULL;

//                 if (new_head == NULL) {
//                     new_head = new_node;
//                     new_current = new_node;
//                 } else {
//                     new_current->next = new_node;
//                     new_current = new_current->next;
//                 }

//                 joined_content = NULL;  // Reset joined_content for next IN_D_QUOT state
//             } else {
//                 // Join the content within double quotes
//                 if (joined_content == NULL) {
//                     joined_content = strdup(current->content);
//                 } else {
//                     joined_content = realloc(joined_content, strlen(joined_content) + strlen(current->content) + 1);
//                     strcat(joined_content, current->content);
//                 }

//                 t_info* temp = current;
//                 current = current->next;
//                 free(temp->content);
//                 free(temp);
//                 continue;
//             }
//         }

//         current = current->next;
//     }

//     if (joined_content != NULL) {
//         // Create a new node for the joined content within double quotes
//         t_info* new_node = (t_info*)malloc(sizeof(t_info));
//         new_node->content = joined_content;
//         new_node->type = QUOT;
//         new_node->state = IN_QUOT;
//         new_node->next = NULL;

//         if (new_head == NULL) {
//             new_head = new_node;
//             new_current = new_node;
//         } else {
//             new_current->next = new_node;
//             new_current = new_current->next;
//         }
//     }

//     return new_head;
// }

t_info *join_content(t_info *info)
{
    t_info *tmp = info;

    while (tmp)
    {
        if (tmp->type == WORD && tmp->state == NORMAL)
        {
            while (tmp->next->type == WORD && tmp->next->state == NORMAL)
            {
                char *tmp_str;
                tmp_str = ft_strjoin(tmp->content, tmp->next->content);
                free(tmp->content);
                tmp->content = ft_strdup(tmp_str);
                free(tmp_str);
                tmp->state = NORMAL;
                tmp->type = WORD;
                info = delete_node(info, tmp->next);
            }
        }
        else if (tmp->state == IN_QUOT)
        {
            while (tmp->next->state == IN_QUOT)
            {
                char *tmp_str;
                tmp_str = ft_strjoin(tmp->content, tmp->next->content);
                free(tmp->content);
                tmp->content = ft_strdup(tmp_str);
                free(tmp_str);
                tmp->state = IN_QUOT;
                tmp->type = WORD;
                info = delete_node(info, tmp->next);
            }
        }
        else if (tmp->type != VAR && tmp->type != EXIT_S && tmp->state == IN_D_QUOT)
        {
            while (tmp->next->type != VAR && tmp->next->type != EXIT_S && tmp->next->state == IN_D_QUOT)
            {
                char *tmp_str;
                tmp_str = ft_strjoin(tmp->content, tmp->next->content);
                free(tmp->content);
                tmp->content = ft_strdup(tmp_str);
                free(tmp_str);
                tmp->state = NORMAL;
                tmp->type = WORD;
                info = delete_node(info, tmp->next);
            }
        }
        tmp = tmp->next;
    }
    return info;
}

int main()
{
    extern char **environ;
    t_node *head = NULL;
    char *input;

    head = creat_env(head, environ);
    while(1)
    {
        t_info *info = NULL;
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
        info = set_state(info);
        // print_list(info);
        info = remove_quots(info);
        // info = recreate_linked_list(info);
        // print_list(info);
        if(!cheak_quoting(info))
            continue;
        info = join_content(info);
        print_list(info);
        free_list(info);
        // arr = check_quoting(input);
        // tab = split(input, ' ');
        // head = commands(tab, head, arr);
        // free_tab(tab);
        // free(input);
    }
    return 0;
}
