/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parssing_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 13:19:50 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/07 12:36:39 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_tab(char **tab)
{
    int i = 0;
    while (tab[i] != NULL)
        free(tab[i++]);
    free(tab);
}

int slen(char *str)
{
    int i = 0;
    while (str[i])
        i++;
    return i;
}

int nbr_words(char *str, char c)
{
    int counter = 0;
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] != c && (str[i + 1] == c || str[i + 1] == '\0'))
            counter++;
        i++;
    }
    return counter;
}

int word_len(char *str, char c)
{
    int i = 0;

    while (str[i] != '\0')
    {
        if (str[i] != c && (str[i + 1] == c || str[i + 1] == '\0'))
            return i + 1;
        i++;
    }
    return 0;
}

char **split(char *str, char c)
{
    if(!str)
        return NULL;

    char **arr = malloc(sizeof(char *) * (nbr_words(str, c) + 1));

    int i = 0;
    int index = 0;
    while (str[i] != '\0')
    {
        while (str[i] == c)
            i++;
        int j = 0;
        if (str[i] == '\0')
            break;
        arr[index] = malloc(sizeof(char) *( word_len(str + i, c) + 1));
        while (str[i] != c && str[i] != '\0')
        {
            arr[index][j] = str[i];
            j++;
            i++;
        }
        arr[index][j] = '\0';
        index++;
    }
    arr[index] = NULL;
    return arr;
}
