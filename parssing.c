/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parssing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 10:05:20 by ybourais          #+#    #+#             */
/*   Updated: 2023/07/06 17:55:32 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_error(int n)
{
    write(n, "Error\n", 6);
    exit(n);
}

int valid_dollar(char *str)
{
    int i;
    int quote;
    int counter = 0;

    i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '$')
            counter++;
        if(str[i] == 34 || str[i] == 39)
        {
            quote = str[i++];
            while (str[i] != quote)
            {
                if(str[i] == '\0')
                    return -1;
                else if (str[i] == '$')
                    counter++;
                i++;
            }
        }
        i ++;
    }
    return counter;
}

int arr_dollar(int *arr, int quote, int indice)
{
    if (quote == 34)
        arr[indice++] = quote;
    else if(quote == 39)
        arr[indice++] = quote;
    else if (quote == 0)
        arr[indice++] = -1;
    return indice;
}

int is_valid(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] != 34 && str[i] != 39)
            i++;
        else if (str[i] == 34 || str[i] == 39)
        {
            int quot = str[i];
            while (str[i] != quot)
            {
                if (str[i] == '\0')
                    return 0;
                i++;
            }
            i++;
        }
    }
    return 1;
}

int *check_quoting(char *str)
{
    int i;
    int j = 0;
    int quote = 0;
    int *arr;
    int k = 0;

    arr = malloc(sizeof(int) * valid_dollar(str));
    if(!arr)
        ft_error(1);

    i = 0;
    while (str[i] != '\0')
    {
        if (str[i] != 34 && str[i] != 39)
        {
            if (str[i] == '$')
            {
                quote = 0;
                k = arr_dollar(arr, quote, k);
            }
            str[j] = str[i];
        }
        else if(str[i] == 34 || str[i] == 39)
        {
            quote = str[i++];
            if (quote == 34 && str[i] == 34 && str[i - 1] == ' ' && (str[i + 1] == ' ' || str[i + 1] == '\0'))
                str[j++] = 25;
            while (str[i] != quote)
            {
                if(str[i] == '\0')
                    ft_error(1);
                else if (str[i] == '$')
                    k = arr_dollar(arr, quote, k);
                str[j++] = str[i++];
            }
            j--;
        }
        i ++;
        j ++;
    }
    str[j] = '\0';
    return arr;
}

void modification(char **tab, int n, int t)
{
    if (tab == NULL)
        return ;

    int i = 0;
    while (tab[i] != NULL)
    {
        int j = 0;
        while (tab[i][j] != '\0')
        {
            if (tab[i][j] == n)
                tab[i][j] = t;
            j++;
        }
        i++; 
    }
}
