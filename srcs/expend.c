/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expend.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 18:11:03 by tamigore          #+#    #+#             */
/*   Updated: 2022/02/27 14:21:43 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void del_unquot_extra(char *str, int *i, int *j, char q)
{
	int len;
	
	if (str[*j] == q && str[*i] == q)
	{
		while (str[*j])
		{
			str[*j] = str[*j + 1];
			(*j)++;
		}
		len = --(*i) - 1;
		while (str[*i])
		{
			str[*i] = str[*i + 1];
			(*i)++;
		}
		str[*i - 1] = '\0';
		*i = len;
	}
}

char	*del_unused_quot(char *str)
{
	char	*res;
	char	quot;
	int		i;
	int		j;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			quot = str[i];
			j = i++;
			while (str[i] && str[i] != quot)
				i++;
			del_unquot_extra(str, &i, &j, quot);
		}
		else
			i++;
	}
	res = ft_strdup(str);
	if (str)
		free(str);
	return (res);
}

static char	*replace_str(char *str, char *new, int i, int j)
{
	char	*res;
	int		x;
	int		y;
	int		z;

	res = ft_strnew(ft_strlen(str) - (i - j) + ft_strlen(new));
	if (!res || !new)
	{
		if (res)
			free(res);
		if (new)
			free(new);
		if (str)
			free(str);
		return (NULL);
	}
	y = 0;
	x = 0;
	while (str[y] && y < j)
		res[x++] = str[y++];
	if (new)
	{
		z = 0;
		while (new[z])
			res[x++] = new[z++];
	}
	y += i - j;
	ft_strcat(&res[x], &str[y]);
	if (str)
		free(str);
	return (res);
}

char	*expend_words(char *str, int i)
{
	char	*util;
	int		j;
	t_env	*var;
	char	*res;

	util = NULL;
	res = ft_strdup(str);
	if (str)
		free(str);
	if (!res)
		return (NULL);
	j = i + 1;
	if (res[j] == '?')
	{
		var = handler(-1, NULL, NULL, "?");
		if (var)
		{
			util = ft_strdup(var->val);
			if (var->val)
				free(var->val);
			free(var);
		}
		j++;
	}
	else if (res[j] == '$')
	{
		util = ft_itoa(getpid());
		j++;
	}
	else
	{
		while (ft_isalnum(res[j]) || res[j] == '_')
			j++;
		util = ft_strndup(&res[i + 1], (j - (i + 1)));
		if (util)
		{
			var = handler(3, NULL, util, NULL);
			free(util);
			if (var)
				util = ft_strdup(var->val);
			else
				util = NULL;
		}
	}
	res = replace_str(res, util, j, i);
	if (util)
		free(util);
	if (!res)
		return (NULL);
	printf("res = %s\n", res);
	return (del_unused_quot(res));
}