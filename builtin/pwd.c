/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:28:07 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/21 13:15:13 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void pwd(void)
// {
//     char cwd[1024];
//     if (getcwd(cwd, sizeof(cwd)) != NULL)
//     {
//         printf("%s\n", cwd);
//         exist_status = 0;
//     }
//     else
//         exist_status = 1;
// }

void pwd(void)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        write(1, cwd, ft_strlen(cwd));
        write(1, "\n", 1);
        // printf("%s\n", cwd);
        exist_status = 0;
    }
    else
        exist_status = 1;
}