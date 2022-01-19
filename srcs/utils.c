/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 16:47:15 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/19 17:31:53 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_nameval(char *name, char *val)
{
	int		i;
	int		j;
	char	*res;
	
	i = 0;
	res = malloc(sizeof(char) * (ft_strlen(name) + ft_strlen(val) + 2));
	if (!res)
		return (NULL);
	j = i;
	while (name[j])
		res[i++] = name[j++];
	res[i++] = '=';
	j = 0;
	while (val[j])
		res[i++] = val[j++];
	res[i] = '\0';
	return (res);
}

int	count_elem(t_env *env)
{
	t_env	*tmp;
	int		i;

	i = 0;
	tmp = env;
	while (tmp)
	{
		tmp = tmp->next;
		i++;
	}
	return (i);
}

char	**get_env(t_env *env)
{
	int		i;
	char	**all;
	t_env	*tmp;

	if (!env)
		return (NULL);
	i = count_elem(env);
	all = malloc(sizeof(char *) * (i + 1));
	if (!all)
		return (NULL);
	i = 0;
	tmp = env;
	while (tmp)
	{
		all[i] = join_nameval(tmp->name, tmp->val);
		if (!all[i])
			return (NULL);
		tmp = tmp->next;
		i++;
	}
	all[i] = NULL;
	return (all);
}

char	*get_name(char *ret)
{
	int	i;

	i = 0;
	if (ret)
	{
		while (ret[i] && ret[i] != '=')
			i++;
	}
	if (!ret || !ret[i])
		return (NULL);
	return (ft_strndup(ret, i));
}

char	*get_value(char *ret)
{
	while (ret != NULL && *ret && *ret != '=')
		ret++;
	if (!ret)
		return (NULL);
	ret++;
	return (ft_strdup(ret));
}