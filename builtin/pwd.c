/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:28:07 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/21 18:38:04 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pwd(t_cmd *cmd)
{
	char	cwd[1024];

	handle_redirection(*cmd);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		write(1, cwd, ft_strlen(cwd));
		write(1, "\n", 1);
		exist_status = 0;
	}
	else
		exist_status = 1;
	dup2(cmd->old_out, 1);
	close(cmd->old_out);
}
