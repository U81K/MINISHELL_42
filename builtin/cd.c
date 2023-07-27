/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 12:03:03 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/27 01:34:20 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*change_env(t_env *head, char *new, char *old)
{
	t_env	*tmp;

	tmp = head;
	while (tmp)
	{
		if (compare(tmp->key, old))
		{
			free(tmp->value);
			tmp->value = ft_strdup(new);
		}
		tmp = tmp->next;
	}
	return (head);
}

void	print_cd_error(char *str)
{
	write(2, "my_Shell: ", 10);
	write(2, str, ft_strlen(str));
	write(2, " : No such file or directory\n", 29);
	g_exit_status = 1;
}

void	cd(char **tab, t_env *head)
{
	char	cwd[1024];

	if (tab[0] && !tab[1])
	{
		head = change_env(head, getcwd(cwd, sizeof(cwd)), "OLDPWD");
		chdir(getenv("HOME"));
		head = change_env(head, getcwd(cwd, sizeof(cwd)), "PWD");
		g_exit_status = 0;
	}
	else if (tab[1])
	{
		if (!access(tab[1], F_OK))
		{
			head = change_env(head, getcwd(cwd, sizeof(cwd)), "OLDPWD");
			chdir(tab[1]);
			head = change_env(head, getcwd(cwd, sizeof(cwd)), "PWD");
			g_exit_status = 0;
		}
		else
			print_cd_error(tab[1]);
	}
}
