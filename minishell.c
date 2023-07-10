/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 10:59:39 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/10 13:05:43 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *find_value_dollar(t_node *head, char *str)
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
        {
            int indice = strchrch(curr->var, '=') + 1;
            char *find = malloc(sizeof(char) * (slen(curr->var) - indice) + 1);
            int i = 0;
            while (i < slen(curr->var) - indice)
            {
                find[i] = curr->var[indice + i];
                i++;
            }
            find[i] = '\0';
            return find;
        }
    }
    return NULL;
}

char **resize_and_find(char **tab, t_node *head, int i)
{
    char str[slen(tab[i])];
    int j = 0;
    while (j < slen(tab[i]) - 1)
    {
        str[j] = tab[i][j + 1];
        j++;
    }
    str[j] = '\0';
    if(find_value(head, str))
    {
        free(tab[i]);
        tab[i] = find_value_dollar(head, str);
    }
    else
    {
        free(tab[i]);
        tab[i] = malloc(sizeof(char));
        copy_str(tab[i], " ");
    }
    return tab;
}

// char **tab_mod(char **tab)
// {
//     char **table;
//     int counter = 0;
//     int i = 0;
//     while (tab[i])
//     {
//         if (tab[i] == ' ' || tab[i] == '$' || tab[i] == '.')
//             counter ++;
//         i++;
//     }
//     table = malloc(sizeof(char *) * counter + 1);
    
// }

char **var_expantion(int *arr, char **tab, t_node *head, int h)
{
    int i = 0;
    int k = 0;

    i = h;
    while (tab[i])
    {
        if(search(tab[i], '$') && arr[k] == 34)
        {
            k++;
            tab = resize_and_find(tab, head, i);
        }
        else if(search(tab[i], '$') && arr[k] == -1)
        {
            k++;
            tab = resize_and_find(tab, head, i);
        }
        else if(search(tab[i], '$') && arr[k] == 39 )
            k++;
        i++;
    }
    return tab;
}

char **from_list_to_tab(t_node *head)
{
    char **tab;
    t_node *tmp;
    tmp = head;
    
    int i = 0;
    while (tmp)
    {
        tmp = tmp->next;
        i++;   
    }
    tab = malloc(sizeof(char *) * i + 1);
    tmp = head;
    i = 0;
    while (tmp)
    {
        tab[i] = malloc(sizeof(char) * slen(tmp->var) + 1);
        copy_str(tab[i], tmp->var);
        i++;
        tmp = tmp->next;
    }
    tab[i] = NULL;
    return tab;
}

char *find_path(char **env, int j, char *str)
{   
    char *paths = NULL;
    int i = 0;

    if(!env)
        return NULL;

    while (env[i])
    {
        if(compare_until(env[i], str, j))
        {
            paths = malloc(sizeof(char) * (slen(env[i]) + 1 - slen(str)));
            copy_str(paths, env[i] + 5);
            break;
        }
        i++;
    }
    return paths;
}

// void exit_status(char **tab)
// {
//     if(tab[0][0] == '$' && tab[0][1] == '?' && slen(tab[0]) == 2)
//     {
        
//     }   
// }

void exucution(int *arr ,char **tab, t_node *head)
{
    char **env = from_list_to_tab(head);
    char **paths;
    char *path;
    int i = 0;
    path = find_path(env, 4, "PATH");
    paths = split(path, ':');

    i = 0;
    int pid = fork();

    if(pid == 0)
    {
        if(tab[0][0] == '/' || tab[0][0] == '.')
            execve(tab[0], tab, env);
        else
        {
            i = 0;
            tab = var_expantion(arr, tab, head, 0);
            while (paths[i])
            {
                char *cmd_slash = join(paths[i], "/");
                char *cmd = join(cmd_slash, tab[0]);
                free(cmd_slash);
                if(execve(cmd, tab, env) == -1)
                    free(cmd);
                i ++;
            }
            write(2, "command not found\n", 18);
            exit(1);
        }       
    }
    wait(NULL);
    free_tab(env);
    free(path);
    free(paths);
}

int	a_toi(char *str, int *handler)
{
	int		i;
	long	res;

	i = 0;
	res = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '+')
		i++;
	else if (!(str[i] >= '0' && str[i] <= '9'))
		*handler = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
    if(!(str[i - 1] >= '0' && str[i - 1] <= '9'))
        *handler = 0;
	return (res);
}

void ft_exit(char **tab)
{
    if(tab[0] && !tab[1])
    {
        write(1, "exit\n", 5);
        exit(0);
    }
    else if(tab[0] && tab[1] && !tab[2])
    {
        int handler = 1;
        int num = a_toi(tab[1], &handler);
        if(handler == 0)
        {
            write(1, "exit\n", 5);
            write(2, "my_shell: numeric argument required\n", 36);
            exit(255);
        }
        else
        {
            write(1, "exit\n", 5);
            exit(num);
        }
    }
    else if(tab[0] && tab[2])
    {
        perror("exit: too many argument");
    }
}

t_node *commands(char **tab, t_node *head, int *arr)
{
    (void)arr;
    modification(tab, 25, ' ');
    tab = var_expantion(arr, tab, head, 1);
    if (compare(tab[0], "export"))
    {
        head = ft_export(tab, head);
    }
    else if (compare(tab[0], "echo") && !compare(tab[1], "-n"))
        echo(tab);
    else if (compare(tab[0], "echo") && compare(tab[1], "-n"))
        echo_n(tab);
    else if (compare(tab[0], "env") && !tab[1])
        env(head);
    else if (compare(tab[0], "unset") && tab[1])
        head = unset(tab, head);
    else if (compare(tab[0], "pwd"))
        pwd(head);
    else if (compare(tab[0], "exit"))
        ft_exit(tab);
    else if (compare(tab[0], "cd"))
        cd(tab, head);
    else// env ls (ls exucute)
        exucution(arr, tab, head);
    return(head);
}

t_node *creat_env(t_node *head, char **env)
{
    int i = 0;
    while (env[i])
        head = creat_list(head, env[i++]);
    return head;
}

// int main(int ac, char **av, char **env)
// {
//     (void)ac;
//     (void)av;

//     char *input;
//     char **tab;
//     int *arr = {0};
//     t_node *head = NULL;

//     head = creat_env(head, env);
//     while(1)
//     {
//         input = readline("\e[1;32mmy_Shell-310$ \e[0m");
//         if(!input)
//         {
//             printf("\n");
//             break;
//         }
//         if(input[0] == '\0')
//         {
//             free(input);
//             continue;
//         }
//         add_history(input);
//         char *test;
//         test = malloc(sizeof(char) * slen(input) + 1);
//         copy_str(test, input);
//         arr = check_quoting(input);
//         tab = split(input, ' ');
//         head = commands(tab, head, arr);
//         free_tab(tab);
//         free(input);
//     }
//     return 0;
// }
