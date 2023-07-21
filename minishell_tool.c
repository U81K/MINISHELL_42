/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tool.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 22:37:25 by bgannoun          #+#    #+#             */
/*   Updated: 2023/07/21 22:38:32 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_cd(char *input)
{
	if(!input)
	{
	    printf("\n");
	    return (1);
	}
	return (0);
}

int	check_quot(t_info *info)
{
	if(!check_quoting(info))
	{
		free_list(info);
		return (1);
	}
	return (0);
}

int	verifie_syntax(t_info *info)
{
	if(!cheack_syntax(info))
	{
		free_list(info);
		return (1);
	}
	return (0);
}


