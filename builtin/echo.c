/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:29:32 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/21 18:30:03 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	echo(char **tab, t_cmd *cmd)
{
	int	i;
	int	flage;

	handle_redirection(*cmd);
	i = 1;
	flage = 0;
	while (tab[i] && is_arg(tab[i]))
	{
		flage = 1;
		i++;
	}
	while (tab[i])
	{
		write(1, tab[i], ft_strlen(tab[i]));
		if (tab[i + 1] != NULL)
			write(1, " ", 1);
		i++;
	}
	if (!flage)
		write(1, "\n", 1);
	exist_status = 0;
	dup2(cmd->old_out, 1);
	close(cmd->old_out);
}
