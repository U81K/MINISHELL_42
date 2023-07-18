/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 11:00:07 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/18 13:35:28 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <fcntl.h> /* open */
# include <stdio.h> /* printf strerror perror*/
#include <readline/readline.h> /* redline */
#include <readline/history.h> /* redline */
#include <stdlib.h> /* malloc, free getenv */
#include <unistd.h> /* access, fork, getcwd, chdir, unlink execve dup dup2 pipe isatty ttyname ttyslot*/
#include <sys/wait.h> /* waitpid */
#include <sys/stat.h> /* stat */
#include <dirent.h> /* opendir readdir closedir*/
#include <sys/ioctl.h> /* ioctl */
#include <termios.h> /* tcsetattr */
#include <termcap.h> /* tgetent */
#include "Libft/libft.h"
#include <string.h>

int exist_status;

typedef struct  s_tool 
{
    char **paths;
    char *path;
    char **env;
    int pid;
    int handler;
    int i;
} t_tool;

typedef struct  s_list 
{
    char *key;
    char *value;
    struct s_list *next;
} t_env;

typedef enum e_type
{
    QUOT = '\'',
    D_QUOT = '\"',
    S_SPACE = ' ',
    VAR = '$',
    PIPE = '|',
    R_OUT = '>',
    R_IN = '<',
    EXIT_S = '?',
    WORD,
    DR_IN,
    DR_OUT,
} t_type;

typedef enum e_state
{
    IN_QUOT,
    IN_D_QUOT,
    NORMAL,
} t_state;

typedef struct  s_info 
{
    char *content;
    t_type type;
    t_state state;
    struct s_info *next;
} t_info;

typedef struct  s_rd
{
    char *file;
    int type;
    struct s_rd *next;
} t_rd;

typedef struct  s_cmd
{
    char **full_cmd;
    int nbr_arg;
    t_rd *rd;
} t_cmd;

/*=====================================*/
/*===============PARSSING==============*/
/*=====================================*/

/*LEXER*/
t_info *handle_dollar_sign(t_info *info, char *line, int *i);
t_info *handle_space(t_info *info, char *line, int *i);
t_info *handle_word(t_info *info, char *line, int *i);
t_info *handle_pipe(t_info *info, char *line, int *i);
t_info *lexer(t_info *info, char *line);

/*LEXER_TOOLS*/
int is_space(char c);
int special_char(char c);
char *get_type(t_type type);
char *get_state(t_state state);

/*LEXER_2*/
t_info *handle_output_redirection(t_info *info, char *line, int *i);
t_info *handle_inpute_redirection(t_info *info, char *line, int *i);
t_info *handle_quote(t_info *info, char *line, int *i);
t_info *handle_double_quote(t_info *info, char *line, int *i);
t_info *set_state(t_info *info);

/*check_syntax_2*/
t_info *remove_quots(t_info *info);
int check_quoting(t_info *info);
int cheak_pipes(t_info *info);
int cheack_red(t_info *info);
int cheack_syntax(t_info *info);

/*parssing*/
t_info *join_content(t_info *info);
t_cmd *parss_redirection(t_info *info);
t_rd *creat_redirection(t_rd *head, char *file, int type);
t_cmd *get_cmd_and_args(t_cmd *cmd, t_info *info);
void joind_and_free_next(t_info *node, t_state state, t_info *to_delete);

/*expand*/
t_info *expand_variable(t_info *info, t_env *env);
t_info *expand_existing_var(t_info *info, t_env *env);
t_info *expand_var(t_env *env, t_info *info);
t_info *if_space_is_there(char *str, t_info *info);
t_info *process_dollar_after_expand(t_info *info);
t_info *process_exit_status(t_info *info);

/*parssing_tools*/
int compare(char *s1, char *s2);
int nbr_cmd(t_info *info);
t_cmd *cmd_init(t_cmd *cmd, t_info *info, int nbr);
t_cmd *nbr_arg(t_info *info, t_cmd *cmd);

/*tools*/
t_info* delete_node(t_info* head, t_info* to_delete);
t_info *creat_node(t_info *head, char *content, t_type type, t_state state);
t_env *creat_liste(t_env *head, char *env);
t_env *ft_env(char **tab);

/*tools_2*/
void copy_str(char *dst, char *src);
void print_list(t_info *head);
void free_list(t_info *head);
char *set_variables(char *str);
char *set_value(char *str);


/*=====================================*/
/*===============exucution=============*/
/*=====================================*/


/*pipes*/
t_env *run_commands(t_cmd *cmd, t_env* env, t_info *info);
void redirect_fd_to_pipe_and_close(int num_of_cmd, int(*fd)[2], int index);
void close_fd(int num, int (*fd)[2]);
void creat_pipes(int numb_of_cmd, int (*fd)[2]);
void wait_for_child(int numb_of_cmd, int (*fd)[2], int *pid);


/*exucution*/
t_env *commands(t_cmd *cmd, t_env* env, t_info *info);
void exucution(t_cmd cmd, t_env *env);
void free_list_cmd(t_cmd *head, t_info *info);
void free_red(t_rd *head);
int exist_or_not(char *str, char c);

/*exucution*/
void to_write_error(char *cmd, char *msg, int len, int exit_status);
int is_path(t_cmd cmd);
void free_and_wait(int pid, char **env, char **paths, char *path);
t_tool *get_info(t_tool *tool, t_env *environ);
void search_and_exece(t_tool *tool, t_cmd *cmd);


/*exucution_utils*/
int compare_until(char *s1, char *s2, int n);
char *find_path(char **env, int j, char *str);
void free_tab(char **tab);
int nbr_of_element(t_env *env);
char **from_list_to_tab(t_env *head);

/*export*/
t_env *ft_export(t_cmd cmd, t_env *env);
void print_export(t_env *env);
void print_export_error(char *str);
t_env *every_thing_all_at_once(t_env *env, char *str, int *equal);

/*export_tools*/
int is_valid(char *str);
t_env *search_list(t_env *head, char *str);
int find_value_in_list(t_env *env, char *str);
t_env *add_to_env(t_env *head, char *key, char *value, int p);

/*env*/
void print_env(t_env *head);

/*cd*/
void cd(char **tab, t_env *head);
t_env *change_env(t_env *head, char *new, char *old);
void print_cd_error(char *str);

/*pwd*/
void pwd(void);

/*unset*/
t_env *unset(t_cmd cmd, t_env *env);
t_env *unset_node(t_env *head, char *to_delete);
int find_value(t_env *head, char *str);
void free_for_unset(t_env *curr);

/*exit*/
int	a_toi(char *str, int *handler);
void ft_exit(char **tab);

/*echo*/
int	is_arg(char *str);
void echo (char **tab);


#endif