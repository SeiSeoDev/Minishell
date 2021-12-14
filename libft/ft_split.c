/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 13:56:52 by tamigore          #+#    #+#             */
/*   Updated: 2021/12/08 11:54:35 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	void	ft_free_all(char **tab, int x)
{
	int	i;

	i = 0;
	while (i < x)
		free(tab[i++]);
	free(tab);
}

static	int	add_space_to_tab(char **tab, int j, int *x)
{
	tab[*x] = (char *)malloc(sizeof(char) * (j + 1));
	if (!tab[*x])
	{
		ft_free_all(tab, *x);
		return (0);
	}
	(*x)++;
	return (1);
}

static	char	**ft_malloctab(char **tab, const char *s, char c, int x)
{
	int		j;
	int		i;

	i = 0;
	while (s[i])
	{
		j = 0;
		while (s[i] != c && s[i])
		{
			j++;
			i++;
		}
		if (j != 0)
			if (!add_space_to_tab(tab, j, &x))
				return (NULL);
		if (s[i])
			i++;
	}
	return (tab);
}

static	void	ft_filtab(char **tab, const char *s, char c)
{
	int		i;
	int		j;
	int		x;

	i = 0;
	x = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			j = 0;
			while (s[i] != c && s[i])
			{
				tab[x][j++] = s[i++];
			}
			tab[x++][j] = '\0';
		}
		if (s[i])
			i++;
	}
	tab[x] = NULL;
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	char	**tab;

	i = 0;
	j = 0;
	if (!s)
		return (NULL);
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			j++;
		i++;
	}
	tab = (char **)malloc(sizeof(char *) * (j + 1));
	if (!tab)
		return (NULL);
	tab = ft_malloctab(tab, s, c, 0);
	if (!tab)
		return (NULL);
	ft_filtab(tab, s, c);
	return (tab);
}
