/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parssing_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 13:19:50 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/11 19:13:05 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	compare(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (0);
	if (ft_strlen(s1) != ft_strlen(s2))
		return (0);
	while (s1[i] && s1[i])
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}
	return (1);
}

int	nbr_cmd(t_info *info)
{
	t_info	*tmp;
	int		counter;

	tmp = info;
	counter = 0;
	while (tmp)
	{
		if (tmp->type == PIPE || !tmp->next)
			counter++;
		tmp = tmp->next;
	}
	return (counter);
}

t_cmd	*cmd_init(t_cmd *cmd, t_info *info, int nbr)
{
	int	i;

	(void)info;
	i = 0;
	while (i < nbr)
	{
		cmd[i].full_cmd = NULL;
		cmd[i].rd = NULL;
		i++;
	}
	return (cmd);
}

t_cmd	*nbr_arg(t_info *info, t_cmd *cmd)
{
	int	counter;
	int	i;

	counter = 0;
	i = 0;
	while (info)
	{
		if (info->type == PIPE || !info->next)
		{
			cmd[i].nbr_arg = counter;
			if (!info->next)
				cmd[i].nbr_arg = counter + 1;
			counter = -1;
			i++;
		}
		counter++;
		info = info->next;
	}
	return (cmd);
}

void	free_red(t_rd *head)
{
	t_rd	*current;
	t_rd	*next;

	current = head;
	if (!current)
		return ;
	while (current)
	{
		next = current->next;
		free(head->file);
		current = next;
	}
}

// void free_tab(char **tab)
// {
//     int i = 0;
//     while (tab[i] != NULL)
//         free(tab[i++]);
//     free(tab);
// }

// int slen(char *str)
// {
//     int i = 0;
//     while (str[i])
//         i++;
//     return (i);
// }

// int nbr_words(char *str, char c)
// {
//     int counter = 0;
//     int i = 0;
//     while (str[i] != '\0')
//     {
//         if (str[i] != c && (str[i + 1] == c || str[i + 1] == '\0'))
//             counter++;
//         i++;
//     }
//     return (counter);
// }

// int word_len(char *str, char c)
// {
//     int i = 0;

//     while (str[i] != '\0')
//     {
//         if (str[i] != c && (str[i + 1] == c || str[i + 1] == '\0'))
//             return (i + 1);
//         i++;
//     }
//     return (0);
// }

// char **split(char *str, char c)
// {
//     if(!str)
//         return (NULL);

//     char **arr = malloc(sizeof(char *) * (nbr_words(str, c) + 1));

//     int i = 0;
//     int index = 0;
//     while (str[i] != '\0')
//     {
//         while (str[i] == c)
//             i++;
//         int j = 0;
//         if (str[i] == '\0')
//             break ;
//         arr[index] = malloc(sizeof(char) *( word_len(str + i, c) + 1));
//         while (str[i] != c && str[i] != '\0')
//         {
//             arr[index][j] = str[i];
//             j++;
//             i++;
//         }
//         arr[index][j] = '\0';
//         index++;
//     }
//     arr[index] = NULL;
//     return (arr);
// }
