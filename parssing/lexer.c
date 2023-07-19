/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 09:57:41 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/19 10:15:39 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_info	*handle_dollar_sign(t_info *info, char *line, int *i)
{
	int	counter;

	if (line[*i + 1] == '?')
	{
		info = creat_node(info, ft_strdup("$?"), EXIT_S, NORMAL);
		*i += 2;
	}
	else if (ft_isdigit(line[*i + 1]))
	{
		info = creat_node(info, ft_substr(line, *i, 2), VAR, NORMAL);
		*i += 2;
	}
	else
	{
		counter = 0;
		while (ft_isalnum(line[counter + *i + 1]) || line[counter + *i
				+ 1] == '_')
			counter++;
		info = creat_node(info, ft_substr(line, *i, counter + 1), VAR, NORMAL);
		*i = *i + counter + 1;
	}
	return (info);
}

t_info	*handle_space(t_info *info, char *line, int *i)
{
	(void)line;
	info = creat_node(info, ft_strdup(" "), S_SPACE, NORMAL);
	(*i)++;
	return (info);
}

t_info	*handle_word(t_info *info, char *line, int *i)
{
	int	counter;

	counter = 0;
	while (!special_char(line[*i + counter]))
		counter++;
	info = creat_node(info, ft_substr(line, *i, counter), WORD, NORMAL);
	*i = *i + counter;
	return (info);
}

t_info	*handle_pipe(t_info *info, char *line, int *i)
{
	(void)line;
	info = creat_node(info, ft_strdup("|"), PIPE, NORMAL);
	(*i)++;
	return (info);
}

t_info	*lexer(t_info *info, char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
			info = handle_dollar_sign(info, line, &i);
		else if (is_space(line[i]))
			info = handle_space(info, line, &i);
		else if (!special_char(line[i]))
			info = handle_word(info, line, &i);
		else if (line[i] == '>')
			info = handle_output_redirection(info, line, &i);
		else if (line[i] == '<')
			info = handle_inpute_redirection(info, line, &i);
		else if (line[i] == '|')
			info = handle_pipe(info, line, &i);
		else if (line[i] == '\'')
			info = handle_quote(info, line, &i);
		else if (line[i] == '\"')
			info = handle_double_quote(info, line, &i);
	}
	free(line);
	info = set_state(info);
	return (info);
}
