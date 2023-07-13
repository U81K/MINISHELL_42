/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 10:59:39 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/13 20:32:59 by ybourais         ###   ########.fr       */
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
            if(cmd.full_cmd[0][0] == '\0')
            {
                write(2, "command not found\n", 18);
                exit(1);
            }
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
                write(2, "my_Shell: :command not found\n", 29);
                exist_status = 127;
                exit(exist_status);
            }
        }
    }
    int h;
    waitpid(pid, &h, 0);
    exist_status = WEXITSTATUS(h);
    free_tab(env);
    free_tab(paths);
    free(path);
}

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

t_env	*add_to_env(t_env *head, char *key, char *value, int p)
{
	t_env	*new_node;
	t_env	*tmp;

	new_node = (t_env *)malloc(sizeof(t_env));
	new_node->key = ft_strdup(key);
    if(value)
        new_node->value = ft_strdup(value);
    else if(p)
        new_node->value = ft_strdup("");
    else
        new_node->value = NULL;
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
    int equal = 0;

    if (cmd.full_cmd[0] && !cmd.full_cmd[1])
    {
        tmp = env;
        while (tmp)
        {
            printf("declare -x %s", tmp->key);
            if(tmp->value != NULL)
                printf("=\"%s\"\n", tmp->value);
            else
                printf("\n");
            tmp= tmp->next;
        }
        exist_status = 0;
    }
    else
    {
        i = 1;
        while (i < cmd.nbr_arg)
        {
            if(is_valid(cmd.full_cmd[i]))
            {
                if(exist_or_not(cmd.full_cmd[i], '='))
                    equal = 1;
                env = search_list(env, cmd.full_cmd[i]);
                if(!find_value_in_list(env, cmd.full_cmd[i]))
                    env = add_to_env(env, set_variables(cmd.full_cmd[i]), set_value(cmd.full_cmd[i]), equal);
                exist_status = 0;
            } 
            else
            {
                write(2, "my Shell: export: ", 18);
                printf("`%s': not a valid identifier\n", cmd.full_cmd[i]);
                exist_status = 1;
            }
            equal = 0;
            i++;
        }
    }
    return env;
}

void print_env(t_env *head)
{
    t_env *tmp;
    tmp = head;
    while (tmp)
    {
        if(tmp->value)
            printf("%s=%s\n", tmp->key, tmp->value);
        tmp= tmp->next;
    }
    exist_status = 0;
}

void pwd(void)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
        exist_status = 0;
    }
    else
        exist_status = 1;
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

    char cwd[1024];
    if(tab[0] && !tab[1])
    {
        head = change_env(head, getcwd(cwd, sizeof(cwd)), "OLDPWD");
        chdir(getenv("HOME"));
        head = change_env(head, getcwd(cwd, sizeof(cwd)), "PWD");
        exist_status = 0;
    }
    else if(tab[1])
    {
        if(!access(tab[1], F_OK))
        {
            head = change_env(head, getcwd(cwd, sizeof(cwd)), "OLDPWD");
            chdir(tab[1]);
            head = change_env(head, getcwd(cwd, sizeof(cwd)), "PWD");
            exist_status = 0;
        }
        else
        {
            write(2, "my_Shell: ", 10);
            printf("%s : No such file or directory\n", tab[1]);
            exist_status = 1;
        }
    }
}

int find_value(t_env *head, char *str)
{
    t_env *curr;
    curr = head;

    while (curr)
    {
        if(!compare(str, curr->key))
            curr = curr->next;
        else
            return 1;
    }
    return 0;
}

t_env *unset_node(t_env *head, char *to_delete)
{
    t_env *curr;
    t_env *prev;

    curr = head;
    prev = head;

    if(!find_value(head, to_delete))
        return head;
    else
    {
        if(compare(to_delete, curr->key))
        {
            head = curr->next;
            free(curr->key);
            free(curr->value);
            free(curr);
            return head;
        }
        while (!compare(to_delete, curr->key))
        {
            prev = curr;
            curr = curr->next;
        }
        prev->next = curr->next;
        free(curr->key);
        free(curr->value);
        free(curr);
        return head;
    }
    return head;
}

t_env *unset(t_cmd cmd, t_env *env)
{
    int j = 1;
    while (j < cmd.nbr_arg)
    {
        if(is_valid(cmd.full_cmd[j]))
        {
            env = unset_node(env, cmd.full_cmd[j]);
            exist_status = 0;
        }
        else
        {
            write(2, "my_Shell: unset: ", 17);
            printf("`%s': not a valid identifier\n", cmd.full_cmd[j]);
            exist_status = 1;
        }
        j++;
    }
    return env;
}

int	is_arg(char *str)
{
	int	i;

	i = 0;
	if (str[i] != '-' || ft_strlen(str) == 1)
		return (0);
	i++;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void echo (char **tab)
{
    int i = 1; 
    int flage = 0; 

    while (tab[i] && is_arg(tab[i]))
    {
        flage = 1;
        i++;
    }

    while (tab[i])
    {
        printf("%s", tab[i]);
        if(tab[i + 1] != NULL)
            printf(" ");
        i++;
    }
    if(!flage)
        printf("\n");
    exist_status = 0;
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
    if(!(str[i] >= '0' && str[i] <= '9') && str[i] != '\0')
        *handler = 0;
	return (res);
}

void ft_exit(char **tab)
{
    int p = 1;

    if(tab[0] && !tab[1])
    {
        write(1, "exit\n", 5);
        exist_status = 0;
        exit(exist_status);
    }
    else
    {
        a_toi(tab[1], &p);
        if(p == 1 && tab[2])
        {
            write(2, "exit\n", 5);
            write(2, "my_Shell: exit: too many arguments\n", 35);
            exist_status = 1;
        }
        else if(p == 1 && !tab[2])
        {
            write(1, "exit\n", 5);
            exist_status= a_toi(tab[1], &p);
            exit(a_toi(tab[1], &p));
        }
        else if(p == 0)
        {
            write(2, "exit\nmy_Shell: ", 15);
            printf("exit: %s: numeric argument required\n", tab[1]);
            exist_status = 255;
            exit(exist_status);
        }
    }
}

t_env *commands(t_cmd *cmd, t_env* env, t_info *info)
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
        else if (compare(cmd[i].full_cmd[0], "unset"))
            env = unset(cmd[i], env);
        else if (compare(cmd[i].full_cmd[0], "echo"))
            echo(cmd[i].full_cmd);
        else if (compare(cmd[i].full_cmd[0], "exit"))
            ft_exit(cmd[i].full_cmd);
        else
            exucution(cmd[i], env, info);
        i++;
    }
    return env;
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
