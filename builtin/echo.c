/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:29:32 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/21 13:13:19 by ybourais         ###   ########.fr       */
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
void echo (char **tab)
{
    int i = 1; 
    int flage = 0;

    while (tab[i] && is_arg(tab[i]))
    {
        flage = 1;
        i++;
    }

    while (tab[i])
    {
        write(1, tab[i], ft_strlen(tab[i]));
        if(tab[i + 1] != NULL)
            write(1, " ", 1);
        // printf("%s", tab[i]);
        // if(tab[i + 1] != NULL)
        //     printf(" ");
        i++;
    }
    if(!flage)
        write(1, "\n", 1);
    // if(!flage)
    //     printf("\n");
    exist_status = 0;
}

// void echo (char **tab)
// {
//     int i = 1; 
//     int flage = 0;

//     while (tab[i] && is_arg(tab[i]))
//     {
//         flage = 1;
//         i++;
//     }

//     while (tab[i])
//     {
//         printf("%s", tab[i]);
//         if(tab[i + 1] != NULL)
//             printf(" ");
//         i++;
//     }
//     if(!flage)
//         printf("\n");
//     exist_status = 0;
// }
