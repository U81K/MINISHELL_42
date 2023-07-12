/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 12:52:38 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/12 13:42:58 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// t_node *creat_list(t_node *head, char *env)
// {
//     t_node *new_node = malloc(sizeof(t_node));
//     new_node->var = malloc(sizeof(char) * slen(env) + 1);
//     copy_str(new_node->var, env);
//     new_node->next = NULL;
//     if(head == NULL)
//         head = new_node;
//     else
//     {
//         t_node *tmp = head;
//         while (tmp->next != NULL)
//             tmp = tmp->next;
//         tmp->next = new_node;
//     }
//     return head;
// }

// t_node *search_list(t_node *head, char *str, int *p)
// {
//     t_node *tmp;
//     tmp = head;

//     while (tmp)
//     {
//         if (compare(tmp->var, str))
//         {
//             head = replace_node(head, str, tmp->var);
//             *p = 1;
//             return head;
//         }
//         tmp = tmp->next;
//     }
//     *p = 0;
//     return head;
// }

// t_node *replace_node(t_node *head, char *new, char *to_delete)
// {
//     t_node *new_node;
//     new_node = malloc(sizeof(t_node));
//     new_node->var = malloc(sizeof(char) * slen(new) + 1);
//     copy_str(new_node->var, new);

//     t_node *curr;
//     curr = head;
//     if(compare(curr->var, to_delete))
//     {
//         new_node->next = head->next;
//         head = new_node;
//         free(curr->var);
//         free(curr);
//         return head;
//     }
//     t_node *prev;
//     curr = head;
//     prev = head;
//     while (!compare(curr->var, to_delete))
//     {
//         prev = curr;
//         curr = curr->next;
//     }
//     prev->next = new_node;
//     new_node->next = curr->next;
//     free(curr->var);
//     free(curr);
//     return head;
// }

// t_node *unset_node(t_node *head, char *to_delete)
// {
//     t_node *curr;
//     t_node *prev;

//     curr = head;
//     prev = head;

//     if(!find_value(head, to_delete))
//         return head;
//     else
//     {
//         if(compare_len(to_delete, curr->var))
//         {
//             head = curr->next;
//             free(curr->var);
//             free(curr);
//             return head;
//         }
//         while (!compare_len(to_delete, curr->var))
//         {
//             prev = curr;
//             curr = curr->next;
//         }
//         prev->next = curr->next;
//         free(curr->var);
//         free(curr);
//         return head;
//     }
//     return head;
// }

// int find_value(t_node *head, char *str)
// {
//     t_node *curr;
//     t_node *after;

//     curr = head;
//     after = head;

//     while (curr)
//     {
//         if(!compare_len(str, curr->var))
//         {
//             after = curr;
//             curr = after->next;
//         }
//         else
//             return 1;
//     }
//     return 0;
// }

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

// t_env *ft_export(t_cmd *cmd, t_env *head, t_info *info)
// {
//     t_env *tmp;
//     int i = 0;
//     int p = 0;

//     if (tab[0] && !tab[1])
//     {
//         tmp = head;
//         while (tmp)
//         {
//             printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
//             tmp= tmp->next;
//         }
//     }
//     else
//     {
//         i = 1;
//         while (tab[i])
//         {
//             if (is_alphanumeric(tab[i]) || without_equal(tab[i]))
//             {
//                 head = search_list(head, tab[i], &p);
//                 if (!p)
//                     head = creat_list(head, tab[i]);
//             }
//             else
//                 printf("my Shell: no matches found: %s\n", tab[i]);
//             i++;
//         }
//         i = 0;
//         int j = 1;
//         while (i < nbr_cmd(info))
//         {
//             if (is_alphanumeric(cmd[i].full_cmd[j]) || without_equal(cmd[i].full_cmd[j]))
//             {
//                 head = search_list(head, cmd[i].full_cmd[j], &p);
//                 if (!p)
//                     head = creat_list(head, tab[i]);
//             }
//             else
//                 printf("my Shell: no matches found: %s\n", tab[i]);
//             i++;
//         }
//     }
//     return head;
// }

// void echo (char **tab)
// {
//     int i = 0;
//     i = 1;
//     while (tab[i])
//     {
//         printf("%s", tab[i]);
//         if(tab[i][0] != ' ' && tab[i + 1])
//             printf(" ");
//         i++;
//     }
//     printf("\n");
// }

// void echo_n (char **tab)
// {
//     int i = 0;
//     i = 2;
//     while (tab[i])
//     {
//         printf("%s", tab[i]);
//         if(tab[i][0] != ' ' && tab[i + 1])
//             printf(" ");
//         i++;
//     }
// }

// int cheak_exist(char *str, char c)
// {
//     int i = 0;
//     while (str[i])
//     {
//         if (str[i] == c)
//             return 1;
//         i++;
//     }
//     return 0;
// }

// // void env(t_node *head)
// // {
// //     t_node *tmp;
// //     tmp = head;
// //     while (tmp != NULL && strchrch(tmp->var, '=') != slen(tmp->var))
// //     {
// //         printf("%s\n", tmp->var);
// //         tmp= tmp->next;
// //     }
// // }

// void env(t_node *head)
// {
//     t_node *tmp;
//     tmp = head;
//     while (tmp != NULL)
//     {
//         if(cheak_exist(tmp->var, '='))
//         {
//             printf("%s\n", tmp->var);
//         }
//         tmp= tmp->next;
//     }
// }

// t_node *unset(char **tab, t_node *head)
// {
//     int j = 1;
//     while (tab[j])
//     {
//         if(is_alphanumeric(tab[j]) || without_equal(tab[j]))
//             head = unset_node(head, tab[j]);
//         else
//             printf("my Shell: no matches found: %s\n", tab[j]);
//         j++;
//     }
//     return head;
// }

// void pwd(t_node *head)
// {
//     // t_node *curr;
//     // curr = head;
//     // while (curr)
//     // {
//     //     if (curr->var[0] == 'P' && curr->var[1] == 'W' && curr->var[2] == 'D')
//     //     {
//     //         printf("%s\n", curr->var + 4);
//     //         return;
//     //     }
//     //     curr = curr->next;
//     // }

//     // (void)head;
//     // if (getenv("PWD") != NULL)
//     //     printf("%s\n", getenv("PWD"));

//     (void)head;
//     char cwd[1024];
//     if (getcwd(cwd, sizeof(cwd)) != NULL) 
//         printf("%s\n", cwd);
// }

// t_node *change_env(t_node *head, char *new, char *old)
// {
//     t_node *tmp;
//     tmp = head;

//     while (tmp)
//     {
//         if (compare_until(tmp->var, old, 4))
//         {
//             free(tmp->var);
//             tmp->var = malloc(sizeof(char) * slen(new) +  slen(old) + 1);
//             char *str = join(old, new);
//             copy_str(tmp->var, str);
//             free(str);
//         }
//         tmp = tmp->next;
//     }
//     return head;
// }

// void cd(char **tab, t_node *head) 
// {
//     char **env = from_list_to_tab(head);
//     char *path;
//     path = find_path(env, 4, "HOME");

//     char cwd[1024];
//     if(tab[0] && !tab[1])
//     {
//         head = change_env(head, getcwd(cwd, sizeof(cwd)), "OLDPWD=");
//         chdir(path);
//         head = change_env(head, getcwd(cwd, sizeof(cwd)), "PWD=");
//     }
//     else if(tab[0] && tab[1] && !tab[2])
//     {
//         head = change_env(head, getcwd(cwd, sizeof(cwd)), "OLDPWD=");
//         chdir(tab[1]);
//         head = change_env(head, getcwd(cwd, sizeof(cwd)), "PWD=");
//     }
//     else 
//         perror("my_shell");
//     free(path);
// }
