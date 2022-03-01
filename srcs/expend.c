/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expend.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 18:11:03 by tamigore          #+#    #+#             */
/*   Updated: 2022/03/01 02:57:40 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*replace_str(char *str, char *new, int i, int j)
{
	char	*res;
	int		x;
	int		y;
	int		z;

	res = ft_strnew(ft_strlen(str) - (i - j) + ft_strlen(new));
	if (!res)
	{
		if (new)
			free(new);
		free(str);
		return (NULL);
	}
	y = 0;
	x = 0;
	while (str && str[y] && y < j)
		res[x++] = str[y++];
	z = 0;
	while (new && new[z])
		res[x++] = new[z++];
	y += i - j;
	ft_strcat(&res[x], &str[y]);
	if (str)
		free(str);
	return (res);
}

static char	*expend_special(char *res, char *util, int *j)
{
	t_env	*var;

	if (res[(*j)] == '$')
		util = ft_itoa(getpid());
	else if (res[(*j)] == '?')
	{
		var = handler(-1, NULL, NULL, "?");
		if (!var)
			return (NULL);
		util = ft_strdup(var->val);
		free_env(var);
	}
	else
		util = ft_strdup("Minishell");
	(*j)++;
	return (util);
}

static char	*expend_extra(char *res, char *util, int *j, int i)
{
	t_env	*var;

	while (ft_isalnum(res[*j]) || res[*j] == '_')
		(*j)++;
	util = ft_strndup(&res[i + 1], (*j - (i + 1)));
	if (util)
	{
		var = handler(3, NULL, util, NULL);
		free(util);
		if (var)
			return (util = ft_strdup(var->val));
	}
	return (NULL);
}

char	*expend_words(char *str, int i)
{
	char	*util;
	int		j;
	char	*res;

	res = ft_strdup(str);
	if (str)
		free(str);
	if (!res)
		return (NULL);
	j = i + 1;
	if (res[j] == '?' || res[j] == '$' || res[j] == '0')
		util = expend_special(res, NULL, &j);
	else
		util = expend_extra(res, NULL, &j, i);
	res = replace_str(res, util, j, i);
	if (util)
		free(util);
	if (!res)
		return (NULL);
	return (del_unused_quot(res));
}
