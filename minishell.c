/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 10:59:39 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/12 17:49:58 by ybourais         ###   ########.fr       */
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
    while (tab[i])
        free(tab[i++]);
    free(tab);
}

int nbr_of_element(t_env *env)
{
    int counter = 0;
    t_env *tmp = env;
    while (tmp)
    {
        counter ++;
        tmp = tmp->next;
    }
    return counter;
}

char **from_list_to_tab(t_env *env)
{
    t_env *tmp;
    char **tab;
    int i = 0;

    tab = malloc(sizeof(char *) * (nbr_of_element(env) + 1));
    tmp = env;
    i = 0;
    while (tmp)
    {
        tab[i] = malloc(sizeof(char) * (ft_strlen(tmp->key) + ft_strlen(tmp->value) + 2));
        copy_str(tab[i], tmp->key);
        tab[i][ft_strlen(tmp->key)] = '=';
        copy_str(tab[i] + ft_strlen(tmp->key) + 1, tmp->value);
        tmp = tmp->next;
        i++;
    }
    tab[i] = NULL;
    return tab;
}

void exucution(t_cmd cmd, t_env *environ, t_info *info)
{
    int i;
    char **env;
    char **paths;
    char *path;
    (void)info;

    env = from_list_to_tab(environ);
    path = find_path(env, 4, "PATH");
    paths = ft_split(path, ':');

    int pid = fork();

    if(pid == 0)
    { 
        if(cmd.full_cmd[0][0] == '/' || cmd.full_cmd[0][0] == '.')
            execve(cmd.full_cmd[0], cmd.full_cmd, env);
        else
        {
            i = 0;
            while (paths[i])
            {
                char *cmd_slash = ft_strjoin(paths[i], "/");
                char *cmd_ = ft_strjoin(cmd_slash, cmd.full_cmd[0]);
                free(cmd_slash);
                if(execve(cmd_, cmd.full_cmd, env) == -1)
                    free(cmd_);
                i ++;
            }
            write(2, "command not found\n", 18);
            exit(1);
        }
    }
    wait(NULL);
    free_tab(env);
    free_tab(paths);
    free(path);
}

// void print_export(char *str)
// {
//     char begin[strchrch(str, '=') + 1];
//     char after[slen(str) - strchrch(str, '=') + 1];
    
//     int i = 0;
//     while (str[i] != '=' && str[i] != '\0')
//     {
//         begin[i] = str[i];
//         i++;
//     }
//     begin[i] = '\0';
//     i = i + 1;
//     int j = 0;
//     while (str[i] != '\0')
//     {
//         after[j++] = str[i++];
//     }
//     after[j] = '\0';
//     if(strchrch(str, '=') != slen(str))
//         printf("declare -x %s=\"%s\"\n", begin, after);
//     else
//         printf("declare -x %s\n", begin);
// }



int exist_or_not(char *str, char c)
{
    int i = 0;
    while (str[i])
    {
        if(str[i] == c)
            return i;
        i++;
    }
    return 0;
}

// t_env *replace_node(t_env *head, char *new, char *to_delete)
// {
    // t_env *new_node;
    // new_node = malloc(sizeof(t_env));
    // int indix = ft_exist_or_not(new, '=');
    // if(indix != 0)
    // {
    //     new_node->key = malloc(sizeof(char) * (indix + 1));
    //     new_node->value = malloc(sizeof(char) * (ft_strlen(new) - indix + 1));
    //     ft_memcpy(new_node->key, new, indix);
    //     copy_str(new_node->value, new + indix);
    // }
    // else
    // {
    //     new_node->key = malloc(sizeof(char) * (ft_strlen(new) + 1));
    //     copy_str(new_node->key, new);
    //     new_node->value = NULL;
    // }

    // t_env *curr;
    // curr = head;
    // if(compare(curr->key, to_delete))
    // {
    //     new_node->next = head->next;
    //     head = new_node;
    //     free(curr->key);
    //     free(curr->value);
    //     free(curr);
    //     return head;
    // }
    // t_env *prev;
    // curr = head;
    // prev = head;
    // while (!compare(curr->key, to_delete))
    // {
    //     prev = curr;
    //     curr = curr->next;
    // }
    // prev->next = new_node;
    // new_node->next = curr->next;
    // free(curr->key);
    // free(curr->value);
    // free(curr);
//     return head;
// }

int is_valid(char *str)
{
    int j = 0;

    if ((ft_isalpha(str[j]) || str[j] == '_'))
        j++;
    else
        return 0;
    while (str[j] && str[j] != '=')
    {
        if (!ft_isalnum(str[j]) && str[j] != '_')
            return 0;
        j++;
    }
    return 1;
}

t_env *search_list(t_env *head, char *str)
{
    t_env *tmp;
    tmp = head;

    char *key = set_variables(str);
    char *value = set_value(str);

    while (tmp)
    {
        if (compare(tmp->key, key) && value)
        {
            free(tmp->value);
            tmp->value = ft_strdup(value);
            return head;
        }   
        tmp = tmp->next;
    }
    return head;
}

t_env	*add_to_env(t_env *head, char *key, char *value)
{
	t_env	*new_node;
	t_env	*tmp;

	new_node = (t_env *)malloc(sizeof(t_env));
	new_node->key = ft_strdup(key);
    if(value)
        new_node->value = ft_strdup(value);
    else
        new_node->value = ft_strdup("\0");

	new_node->next = NULL;
	if (head == NULL)
		head = new_node;
	else
	{
		tmp = head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
	return (head);
}

int find_value_in_list(t_env *env, char *str)
{
    t_env *tmp = env;
    while (tmp)
    {
        if(compare(tmp->key, set_variables(str)))
            return 1;
        tmp = tmp->next;
    }
    return 0;
}

t_env *ft_export(t_cmd cmd, t_env *env, t_info *info)
{
    t_env *tmp;
    (void)info;
    int i = 0;

    if (cmd.full_cmd[0] && !cmd.full_cmd[1])
    {
        tmp = env;
        while (tmp)
        {
            printf("declare -x %s", tmp->key);
            if(tmp->value[0] != '\0')
                printf("=\"%s\"\n", tmp->value);
            else
                printf("\n");
            tmp= tmp->next;
        }
    }
    else
    {
        i = 1;
        while (i < cmd.nbr_arg)
        {
            if(is_valid(cmd.full_cmd[i]))
            {
                env = search_list(env, cmd.full_cmd[i]);
                if(!find_value_in_list(env, cmd.full_cmd[i]))
                    env = add_to_env(env, set_variables(cmd.full_cmd[i]), set_value(cmd.full_cmd[i]));
            } 
            else
            {
                write(2, "my Shell: export: ", 18);
                printf("`%s': not a valid identifier\n", cmd.full_cmd[i]);
            }
            i++;
        }
    }
    return env;
}

void print_env(t_env *head)
{
    t_env *tmp;
    tmp = head;
    while (tmp && tmp->value[0] != '\0')
    {
        printf("%s=%s\n", tmp->key, tmp->value);
        tmp= tmp->next;
    }
}

void pwd(void)
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

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) 
        printf("%s\n", cwd);
}


// char *find_path(char **env, int j, char *str)
// {   
//     char *paths = NULL;
//     int i = 0;

//     if(!env)
//         return NULL;

//     while (env[i])
//     {
//         if(compare_until(env[i], str, j))
//         {
//             paths = malloc(sizeof(char) * (ft_strlen(env[i]) + 1 - ft_strlen(str)));
//             copy_str(paths, env[i] + 5);
//             break;
//         }
//         i++;
//     }
//     return paths;
// }

t_env *change_env(t_env *head, char *new, char *old)
{
    t_env *tmp;
    tmp = head;

    while (tmp)
    {
        if (compare(tmp->key, old))
        {
            free(tmp->value);
            tmp->value = ft_strdup(new);
        }
        tmp = tmp->next;
    }
    return head;
}

void cd(char **tab, t_env *head) 
{
    // char **env = from_list_to_tab(head);
    // printf("%s\n", );
    
    char cwd[1024];
    if(tab[0] && !tab[1])
    {
        head = change_env(head, getcwd(cwd, sizeof(cwd)), "OLDPWD=");
        chdir(getenv("HOME"));
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
    // free(path);
}

void commands(t_cmd *cmd, t_env* env, t_info *info)
{
    int i = 0;
    while (i < nbr_cmd(info))
    {
        if (compare(cmd[i].full_cmd[0], "export"))
            env = ft_export(cmd[i], env, info);
        else if (compare(cmd[i].full_cmd[0], "env") && !cmd[i].full_cmd[1])
            print_env(env);
        else if (compare(cmd[i].full_cmd[0], "pwd"))
            pwd();
        else if (compare(cmd[i].full_cmd[0], "cd"))
            cd(cmd[i].full_cmd, env);
        else
            exucution(cmd[i], env, info);
        // else if (compare(tab[0], "echo") && !compare(tab[1], "-n"))
        //     echo(tab);
        // else if (compare(tab[0], "echo") && compare(tab[1], "-n"))
        //     echo_n(tab);
        // else if (compare(tab[0], "unset") && tab[1])
        //     head = unset(tab, head);
        // else if (compare(tab[0], "exit"))
        //     ft_exit(tab);
        i++;
    }
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
        info = join_content(info);
        // print_list(info);
        info = remove_space_and_expand(info, env);
        if(!cheack_syntax(info))
        {
            free_list(info);
            continue;
        }
        cmd = parss_redirection(info); // sgft
        cmd = get_cmd_and_args(cmd, info);
        commands(cmd, env, info);
        free_list_cmd(cmd, info);
        free_list(info);
    }
    return 0;
}
