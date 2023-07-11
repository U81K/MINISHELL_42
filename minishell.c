/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 10:59:39 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/11 20:46:43 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int compare_until(char *s1, char *s2, int n)
{
    int i;
    i = 0;
    while (i < n)
    {
        if (s1[i] == s2[i])
            i++;
        else
            return 0;
    }
    return 1;
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
            paths = malloc(sizeof(char) * (ft_strlen(env[i]) + 1 - ft_strlen(str)));
            copy_str(paths, env[i] + 5);
            break;
        }
        i++;
    }
    return paths;
}

// // void exit_status(char **tab)
// // {
// //     if(tab[0][0] == '$' && tab[0][1] == '?' && slen(tab[0]) == 2)
// //     {
        
// //     }   
// // }

// void exucution(int *arr ,char **tab, t_node *head)
// {
//     char **env = from_list_to_tab(head);
//     char **paths;
//     char *path;
//     int i = 0;
//     // exit_status();
//     path = find_path(env, 4, "PATH");
//     paths = split(path, ':');

//     i = 0;
//     int pid = fork();

//     if(pid == 0)
//     {
//         if(tab[0][0] == '/' || tab[0][0] == '.')
//             execve(tab[0], tab, env);
//         else
//         {
//             i = 0;
//             tab = var_expantion(arr, tab, head, 0);
//             if(tab[0] && !tab[1])
//             {
//                 printf("%s\n", tab[0]);
//                 return;
//             }
//             while (paths[i])
//             {
//                 char *cmd_slash = join(paths[i], "/");
//                 char *cmd = join(cmd_slash, tab[0]);
//                 free(cmd_slash);
//                 if(execve(cmd, tab, env) == -1)
//                     free(cmd);
//                 i ++;
//             }
//             write(2, "command not found\n", 18);
//             exit(1);
//         }       
//     }
//     wait(NULL);
//     free_tab(env);
//     free(path);
//     free(paths);
// }

// int	a_toi(char *str, int *handler)
// {
// 	int		i;
// 	long	res;

// 	i = 0;
// 	res = 0;
// 	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
// 		i++;
// 	if (str[i] == '+')
// 		i++;
// 	else if (!(str[i] >= '0' && str[i] <= '9'))
// 		*handler = 0;
// 	while (str[i] >= '0' && str[i] <= '9')
// 	{
// 		res = res * 10 + str[i] - '0';
// 		i++;
// 	}
//     if(!(str[i - 1] >= '0' && str[i - 1] <= '9'))
//         *handler = 0;
// 	return (res);
// }

void free_tab(char **tab)
{
    int i = 0;
    while (tab[i] != NULL)
        free(tab[i++]);
    free(tab);
}

char **from_list_to_tab(t_env *head)
{
    char **tab;

    t_env *tmp;
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
        tab[i] = malloc(sizeof(char) * ft_strlen(tmp->key) + ft_strlen(tmp->value) + 2);
        char *to_free = tab[i];
        tab[i] = ft_strjoin(tmp->key, "=");
        free(to_free);
        tab[i] = ft_strjoin(tab[i], tmp->value);
        tmp = tmp->next;
        i++;
    }
    tab[i] = NULL;
    return tab;
}

void exucution(t_cmd *cmd, t_env *environ, t_info *info)
{
    int i;
    char **env;
    char **paths;
    char *path;

    env = from_list_to_tab(environ);
    path = find_path(env, 4, "PATH");
    paths = ft_split(path, ':');

    int pid = fork();

    int j = 0;
    if(pid == 0)
    {
        while(j < nbr_cmd(info))
        {       
            if(cmd[j].full_cmd[0][0] == '/' || cmd[j].full_cmd[0][0] == '.')
                execve(cmd[j].full_cmd[0], cmd[j].full_cmd, env);
            else
            {
                i = 0;
                while (paths[i])
                {
                    char *cmd_slash = ft_strjoin(paths[i], "/");
                    char *cmd_ = ft_strjoin(cmd_slash, cmd[j].full_cmd[0]);
                    free(cmd_slash);
                    if(execve(cmd_, cmd[j].full_cmd, env) == -1)
                        free(cmd_);
                    i ++;
                }
                write(2, "command not found\n", 18);
                exit(1);
            }
        }
    }
    j++;
    wait(NULL);
    free_tab(env);
    free_tab(paths);
    free(path);
}

void commands(t_cmd *cmd, t_env* env, t_info *info)
{
    // if (compare(tab[0], "export"))
    // {
    //     head = ft_export(cmd, env, info);
    // }
    // else // env ls (ls exucute)
        exucution(cmd, env, info);
    // else if (compare(tab[0], "echo") && !compare(tab[1], "-n"))
    //     echo(tab);
    // else if (compare(tab[0], "echo") && compare(tab[1], "-n"))
    //     echo_n(tab);
    // else if (compare(tab[0], "env") && !tab[1])
    //     env(head);
    // else if (compare(tab[0], "unset") && tab[1])
    //     head = unset(tab, head);
    // else if (compare(tab[0], "pwd"))
    //     pwd(head);
    // else if (compare(tab[0], "exit"))
    //     ft_exit(tab);
    // else if (compare(tab[0], "cd"))
    //     cd(tab, head);
}


int main()
{
    char *input;
    extern char **environ;
    t_env *env = NULL;

    env = ft_env(environ);
    while(1)
    {
        t_info *info = NULL;
        // t_cmd *cmd = NULL;
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
            continue;
        info = remove_quots(info);
        info = join_content(info);
        info = remove_space_and_expand(info, env); // echo $$ ====>> pid of process;
        print_list(info);
        // if(!cheack_syntax(info))
        //     continue;
        // cmd = parss_redirection(info); // sgft
        // cmd = get_cmd_and_args(cmd, info);
        // commands(cmd, env, info);
        // free_list_cmd(cmd, info);
        // free_list(info);
    }
    return 0;
}
