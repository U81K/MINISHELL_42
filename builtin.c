/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 12:52:38 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/08 14:55:02 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node *ft_export(char **tab, t_node *head)
{
    t_node *tmp;
    int i = 0;
    int p = 0;

    if (tab[0] && !tab[1])
    {
        tmp = head;
        while (tmp != NULL)
        {
            print_export(tmp->var);
            tmp= tmp->next;
        }
    }
    else
    {
        i = 1;
        while (tab[i])
        {
            if (is_alphanumeric(tab[i]) || without_equal(tab[i]))
            {
                head = search_list(head, tab[i], &p);
                if (!p)
                    head = creat_list(head, tab[i]);
            }
            else
                printf("my Shell: no matches found: %s\n", tab[i]);
            i++;
        }
    }
    return head;
}

void print_export(char *str)
{
    char begin[strchrch(str, '=') + 1];
    char after[slen(str) - strchrch(str, '=') + 1];
    
    int i = 0;
    while (str[i] != '=' && str[i] != '\0')
    {
        begin[i] = str[i];
        i++;
    }
    begin[i] = '\0';
    i = i + 1;
    int j = 0;
    while (str[i] != '\0')
    {
        after[j++] = str[i++];
    }
    after[j] = '\0';
    if(strchrch(str, '=') != slen(str))
        printf("declare -x %s=\"%s\"\n", begin, after);
    else
        printf("declare -x %s\n", begin);
}

void echo (char **tab)
{
    int i = 0;
    i = 1;
    while (tab[i])
    {
        printf("%s", tab[i]);
        if(tab[i][0] != ' ' && tab[i + 1])
            printf(" ");
        i++;
    }
    printf("\n");
}

void echo_n (char **tab)
{
    int i = 0;
    i = 2;
    while (tab[i])
    {
        printf("%s", tab[i]);
        if(tab[i][0] != ' ' && tab[i + 1])
            printf(" ");
        i++;
    }
}

int cheak_exist(char *str, char c)
{
    int i = 0;
    while (str[i])
    {
        if (str[i] == c)
            return 1;
        i++;
    }
    return 0;
}

// void env(t_node *head)
// {
//     t_node *tmp;
//     tmp = head;
//     while (tmp != NULL && strchrch(tmp->var, '=') != slen(tmp->var))
//     {
//         printf("%s\n", tmp->var);
//         tmp= tmp->next;
//     }
// }

void env(t_node *head)
{
    t_node *tmp;
    tmp = head;
    while (tmp != NULL)
    {
        if(cheak_exist(tmp->var, '='))
        {
            printf("%s\n", tmp->var);
        }
        tmp= tmp->next;
    }
}

t_node *unset(char **tab, t_node *head)
{
    int j = 1;
    while (tab[j])
    {
        if(is_alphanumeric(tab[j]) || without_equal(tab[j]))
            head = unset_node(head, tab[j]);
        else
            printf("my Shell: no matches found: %s\n", tab[j]);
        j++;
    }
    return head;
}

void pwd(t_node *head)
{
    // t_node *curr;
    // curr = head;
    // while (curr)
    // {
    //     if (curr->var[0] == 'P' && curr->var[1] == 'W' && curr->var[2] == 'D')
    //     {
    //         printf("%s\n", curr->var + 4);
    //         return;
    //     }
    //     curr = curr->next;
    // }

    // (void)head;
    // if (getenv("PWD") != NULL)
    //     printf("%s\n", getenv("PWD"));

    (void)head;
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) 
        printf("%s\n", cwd);
}

t_node *change_env(t_node *head, char *new, char *old)
{
    t_node *tmp;
    tmp = head;

    while (tmp)
    {
        if (compare_until(tmp->var, old, 4))
        {
            free(tmp->var);
            tmp->var = malloc(sizeof(char) * slen(new) +  slen(old) + 1);
            char *str = join(old, new);
            copy_str(tmp->var, str);
            free(str);
        }
        tmp = tmp->next;
    }
    return head;
}

void cd(char **tab, t_node *head) 
{
    char **env = from_list_to_tab(head);
    char *path;
    path = find_path(env, 4, "HOME");

    char cwd[1024];
    if(tab[0] && !tab[1])
    {
        head = change_env(head, getcwd(cwd, sizeof(cwd)), "OLDPWD=");
        chdir(path);
        head = change_env(head, getcwd(cwd, sizeof(cwd)), "PWD=");
    }
    else if(tab[0] && tab[1] && !tab[2])
    {
        head = change_env(head, getcwd(cwd, sizeof(cwd)), "OLDPWD=");
        chdir(tab[1]);
        head = change_env(head, getcwd(cwd, sizeof(cwd)), "PWD=");
    }
    else 
        perror("my_shell");
    free(path);
}
