/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:14:38 by ybourais          #+#    #+#             */
/*   Updated: 2022/10/28 16:56:08 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	nbr_word(char const *str, char c)
{
	size_t	i;
	int		nbr;

	nbr = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] != c && (str[i + 1] == c || str[i + 1] == '\0'))
		{
			nbr = nbr + 1;
		}
		i++;
	}
	return (nbr);
}

static int	end_word(char const *str, char c)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] != c && (str[i + 1] == c || str[i + 1] == '\0'))
		{
			return (i + 1);
		}
		i++;
	}
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	size_t	i;
	size_t	j;
	int		counter;

	if (!s)
		return (NULL);
	counter = 0;
	tab = (char **)ft_calloc(nbr_word(s, c) + 1, sizeof(char *));
	if (!tab)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0')
		{
			j = end_word(s + i, c);
			tab[counter++] = ft_substr(s, i, j);
			i = i + j;
		}
	}
	return (tab);
}
