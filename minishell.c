/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 10:59:39 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/21 22:46:56 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit_status = 1;
	}
}

void	signals(void)
{
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
}

t_info	*parssing(t_info **info, t_env **env)
{
	*info = join_content(*info);
	*info = remove_quots(*info);
	*info = expand_variable(*info, *env);
	*info = join_content(*info);
	*info = remove_space(*info);
	return (*info);
}

int	main(void)
{
	char		*input;
	extern char	**environ;
	t_env		*env;
	t_cmd		*cmd;
	t_info		*info;

	env = ft_env(environ);
	while (1)
	{
		input = readline("\e[1;32mmy_Shell-310$ \e[0m");
		if (check_cd(input))
			break ;
		if (ft_strlen(input) > 0)
			add_history(input);
		info = lexer(info, input);
		if (check_quot(info))
			continue ;
		info = parssing(&info, &env);
		if (verifie_syntax(info))
			continue ;
		cmd = parss_redirection(&info);
		cmd = get_cmd_and_args(cmd, info);
		env = run_commands(cmd, env, info);
	}
	return (0);
}
