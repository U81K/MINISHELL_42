/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 11:00:07 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/21 20:35:55 by bgannoun         ###   ########.fr       */
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
    int nbr_cmd;
    int herdoc;
    int old_in;
    int old_out;
} t_cmd;

void	sig_handler(int sig);

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
t_cmd *parss_redirection(t_info **info);
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
void free_red(t_rd *head);

/*tools*/
t_info* delete_node(t_info* head, t_info* to_delete);
t_info *creat_node(t_info *head, char *content, t_type type, t_state state);
t_env *creat_liste(t_env *head, char *env);
t_env *ft_env(char **tab);
void free_list_cmd(t_cmd *head, t_info *info);

/*tools_2*/
void copy_str(char *dst, char *src);
void print_list(t_info *head);
void free_list(t_info *head);
char *set_variables(char *str);
char *set_value(char *str);


/*=====================================*/
/*===============exucution=============*/
/*=====================================*/

/*rediraction*/
void exucute_red_out(char *file_name);
void exucute_double_red_out(char *file_name);
void exucute_red_in(char *file_name);
void handle_redirection(t_cmd cmd);


/*heredoc*/
void	handle_herdoc(t_cmd cmd);
void	handle_heredoc_1(t_rd	*cur, t_cmd cmd, int fd);
int		handle_heredoc_2(t_rd	*cur, int fd);


/*pipes*/
t_env *run_commands(t_cmd *cmd, t_env* env, t_info *info);
void redirect_fd_to_pipe_and_close(int num_of_cmd, int(*fd)[2], int index);
void close_fd(int num, int (*fd)[2]);
void creat_pipes(int numb_of_cmd, int (*fd)[2]);
void wait_for_child(int numb_of_cmd, int (*fd)[2], int *pid);
t_env *fork_multiple_command(t_cmd *cmd, t_env *env, t_info *info, int num);

/*pipes_utils*/
void close_fd(int num, int (*fd)[2]);
void creat_pipes(int numb_of_cmd, int (*fd)[2]);
void wait_for_child(int numb_of_cmd, int (*fd)[2], int *pid);


/*exucution*/
void search_and_exece(t_tool *tool, t_cmd *cmd);
t_env *commands(t_cmd *cmd, t_env* env, t_info *info);
void exucution(t_cmd cmd, t_env *env);
int is_path(t_cmd cmd);
void single_command(t_cmd cmd, t_tool *tool);

/*exucution_2*/
void to_write_error(char *cmd, char *msg, int len, int exit_status);
void free_and_wait(t_tool *tool);
int exist_or_not(char *str, char c);
t_tool *get_info(t_tool *tool, t_env *environ);

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
// void print_env(t_env *head);
void	print_env(t_env *head, t_cmd *cmd);

/*cd*/
void cd(char **tab, t_env *head);
t_env *change_env(t_env *head, char *new, char *old);
void print_cd_error(char *str);

/*pwd*/
void pwd(t_cmd *cmd);

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
void echo (char **tab, t_cmd *cmd);


#endif