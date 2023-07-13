/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:29:32 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/13 17:36:19 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void echo (char **tab)
{
    int i;
    if(compare(tab[1], "-n"))
    {
        i = 2;
        while (tab[i])
            printf("%s", tab[i++]);
    }
    else
    {
        i = 1;
        while (tab[i])
            printf("%s ", tab[i++]);
        printf("\n");
    }
}