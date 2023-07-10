/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 11:44:34 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/10 19:20:24 by ybourais         ###   ########.fr       */
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

// t_node *creat_env(t_node *head, char **env)
// {
//     int i = 0;
//     while (env[i])
//         head = creat_list(head, env[i++]);
//     return head;
// }

void copy_str(char *dst, char *src)
{
    int i = 0;
    while (src[i] != '\0')
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

t_info *creat_node(t_info *head, char *content, t_type type, t_state state)
{
    t_info *new_node = (t_info*)malloc(sizeof(t_info));
    new_node->content = (char *)malloc(sizeof(char) * ft_strlen(content) + 1);
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

// void free_cmd(t_cmd *head) 
// {
//     t_cmd *current = head;
//     t_cmd *next;

//     while (current) 
//     {
//         next = current->next;
//         free(current->content);
//         free(current->content);
//         free(current);
//         current = next;
//     }
// }

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
            tmp = tmp->next;
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

t_info *join_content(t_info *info)
{
    t_info *tmp = info;

    while (tmp  && tmp->next)
    {
        if (tmp->type == WORD && tmp->state == NORMAL)
        {
            while (tmp->next && tmp->next->type == WORD && tmp->next->state == NORMAL)
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
            while (tmp->next && tmp->next->state == IN_QUOT)
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
            while (tmp->next && tmp->next->type != VAR && tmp->next->type != EXIT_S && tmp->next->state == IN_D_QUOT)
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

int compare(char *s1, char *s2)
{
    int i = 0;

    if(!s1 || !s2)
        return 0;
    if(ft_strlen(s1) != ft_strlen(s2))
        return 0;
    while (s1[i] && s1[i])
    {
        if (s1[i] != s2[i])
            return 0;
        i++;
    }
    return 1;
}

t_info *remove_and_expand(t_info *info, t_env *env)
{
    t_info *tmp = info;
    while (tmp)
    {
        if (tmp->type == S_SPACE)
            info = delete_node(info, tmp);
        else if(tmp->type == VAR && (tmp->state == NORMAL || tmp->state == IN_D_QUOT))
        {
            t_env *tmp_env = env;
            while (tmp_env)
            {
                if(compare(tmp_env->key, tmp->content + 1))
                {
                    free(tmp->content);
                    tmp->content = ft_strdup(tmp_env->value);
                    tmp->state = NORMAL;
                    tmp->type = WORD;
                }
                tmp_env = tmp_env->next;
            }
            if(tmp->content[0] == '$' && ft_strlen(tmp->content) != 1)
                info = delete_node(info, tmp);
        }
        tmp = tmp->next;
    }
    return info;
}

char *set_variables(char *str)
{
	char *tab;
	int i = 0;
    while(str[i] && str[i] != '=')
        i++;
    tab = malloc(i+1);
    i = 0;
    while(str[i] && str[i] != '=')
    {
        tab[i] = str[i];
        i++;
    }
    tab[i] = '\0';	
	return(tab);
}

char *set_value(char *str)
{
	char *tab;
	int j = 0;
	int i = 0;
    while(str[i] && str[i] != '=')
        i++;
    if(str[i] == '\0')
        return(NULL);
    tab = malloc(strlen(str) - i);
    i++;
    while(str[i])
        tab[j++] = str[i++];
    tab[j] = '\0';	
	return(tab);
}

t_env *creat_liste(t_env *head, char *env)
{
    t_env *new_node = malloc(sizeof(t_env));
	new_node->key = set_variables(env);
	new_node->value = set_value(env);
    new_node->next = NULL;
    if(head == NULL)
        head = new_node;
    else
    {
        t_env *tmp = head;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = new_node;
    }
    return head;
}

t_env *ft_env(char **tab)
{
	int i = 0;
	t_env *head = NULL;
	while(tab[i])
        head = creat_liste(head, tab[i++]);
	return(head);
}

int cheak_pipes(t_info *info)
{
    t_info *tmp;
    tmp = info;

    if (tmp->type == PIPE)
    {
        write(2, "my_Shell: syntax error near unexpected token ", 45);
        printf("`%s\'\n", tmp->content);
        return 0;
    }
    while (tmp)
    {
        if (tmp->type == PIPE && (!tmp->next || tmp->next->type == PIPE))
        {
            write(2, "my_Shell: syntax error near unexpected token", 45);
            printf("`%s\'\n", tmp->content);
            return 0;
        }
        tmp = tmp->next;
        
    }
    return 1;
}

int cheack_red(t_info *info)
{
    t_info *tmp;
    tmp = info;

    while (tmp)
    {
        if ((tmp->type == R_OUT || tmp->type == R_IN || tmp->type == DR_IN || tmp->type == DR_OUT))
        {
            if(tmp->next && tmp->next->type != WORD)
            {
                write(2, "my_Shell: syntax error near unexpected token `newline\'\n", 56);
                return 0;
            }
            else if(!tmp->next)
            {
                write(2, "my_Shell: syntax error near unexpected token `newline\'\n", 56);
                return 0;
            }
        }
        tmp = tmp->next;
    }
    return 1;
}

int cheack_syntax(t_info *info)
{
    if (!cheak_pipes(info) || !cheack_red(info))
        return 0;
    return 1;
}

int nbr_cmd(t_info *info)
{
    t_info *tmp = info;
    int counter = 0;
    while (tmp)
    {
        if (tmp->type == PIPE || !tmp->next)
            counter ++;
        tmp = tmp->next;
    }
    return (counter);
}

t_rd *creat_red(t_rd *head, char *file, int type)
{
    t_rd *new_node = (t_rd *)malloc(sizeof(t_rd));
    new_node->file = ft_strdup(file);
    new_node->type = type;
    new_node->next = NULL;
    if(head == NULL)
        head = new_node;
    else
    {
        t_rd *tmp = head;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = new_node;
    }
    return head;
}

t_cmd *cmd_init(t_cmd *cmd, t_info *info, int nbr)
{
    (void)info;
    int i = 0;
    while (i < nbr)
    {
        cmd[i].full_cmd = NULL;
        cmd[i].rd = NULL;
        i++;
    }
    return cmd;
}

t_cmd *parss_red(t_info *info)
{
    int cmd_nbr = nbr_cmd(info);
    t_cmd *global = malloc(sizeof(t_cmd) * cmd_nbr);
    t_info *tmp = info;

    global = cmd_init(global, info, cmd_nbr);
    int indice = 0;
    while (tmp)
    {
        if((tmp->type == R_OUT || tmp->type == R_IN || tmp->type == DR_IN || tmp->type == DR_OUT) && tmp->next)
        {
            global[indice].rd = creat_red(global[indice].rd, tmp->next->content, tmp->type);
            info = delete_node(info, tmp->next);
            info = delete_node(info, tmp);
        }
        if (tmp->type == PIPE)
            indice ++;
        tmp = tmp->next;
    }
    return global;
}

t_cmd *nbr_arg(t_info *info, t_cmd *cmd)
{
    int counter = 0;
    int i = 0;
    while (info)
    {
        if(info->type == PIPE || !info->next)
        {
            cmd[i].nbr_arg = counter;
            if(!info->next)
                cmd[i].nbr_arg = counter + 1;
            counter = -1;
            i++;
        }
        counter++;
        info = info->next;
    }
    return cmd;
}

t_cmd *get_cmd_and_args(t_cmd *cmd, t_info *info)
{
    cmd = nbr_arg(info,cmd);
    int num = nbr_cmd(info);
    int i = 0;
    while (i < num)
    {
        cmd[i].full_cmd = malloc(sizeof(char *) * cmd[i].nbr_arg + 1);
        int j = 0;
        while (j < cmd[i].nbr_arg)
        {
            if(info->type != PIPE)
            {
                cmd[i].full_cmd[j] = ft_strdup(info->content);
                j++;
            }
            info = info->next;
        }
        cmd[i].full_cmd[j] = NULL;
        i ++;
    }
    int n = 0;
    while (n < num)
    {
        int k = 0;
        while (k < cmd[n].nbr_arg)
        {
            printf("%s\t", cmd[n].full_cmd[k]);
            k++;
        }
        printf("\n");
        n++;
    }
    return(cmd);
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
        info = set_state(info);
        info = remove_quots(info);
        if(!cheak_quoting(info))
            continue;
        info = join_content(info);
        info = remove_and_expand(info, env);
        if(!cheack_syntax(info))
            continue;
        cmd = parss_red(info);
        cmd = get_cmd_and_args(cmd, info);
        print_list(info);
        free_list(info);
    }
    return 0;
}

    // arr = check_quoting(input);
    // tab = split(input, ' ');
    // head = commands(tab, head, arr);
    // free_tab(tab);
    // free(input);