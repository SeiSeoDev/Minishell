/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 16:47:15 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/18 16:00:07 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_env(char **env)
{
	int i;
	char **all;

	if (!env)
		return (NULL);
	i = 0;
	while (env[i])
		i++;
	all = malloc(sizeof(char *) * (i + 1));
	if (!all)
		return (NULL);
	i = 0;
	while (env[i])
	{
		all[i] = ft_strdup(env[i]);
		if (!all[i])
			return (NULL);
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