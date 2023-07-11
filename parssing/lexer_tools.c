/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 09:58:37 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/11 19:10:10 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\v')
		return (1);
	return (0);
}

int	special_char(char c)
{
	if (is_space(c) || c == '\t' || c == '$' || c == '|' || c == '\''
		|| c == '\"' || c == '>' || c == '<' || c == '\0')
		return (1);
	return (0);
}

char	*get_type(t_type type)
{
	if (type == QUOT)
		return ("QUOT");
	else if (type == D_QUOT)
		return ("D_QUOT");
	else if (type == S_SPACE)
		return ("S_SPACE");
	else if (type == VAR)
		return ("VAR");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == R_OUT)
		return ("R_OUT");
	else if (type == R_IN)
		return ("R_IN");
	else if (type == EXIT_S)
		return ("EXIT_S");
	else if (type == WORD)
		return ("WORD");
	else if (type == DR_IN)
		return ("DR_IN");
	else if (type == DR_OUT)
		return ("DR_OUT");
	return (NULL);
}

char	*get_state(t_state state)
{
	if (state == IN_D_QUOT)
		return ("IN_D_QUOT");
	else if (state == IN_QUOT)
		return ("IN_QUOT");
	else if (state == NORMAL)
		return ("NORMAL");
	return (NULL);
}
