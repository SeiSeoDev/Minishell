/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 17:02:46 by tamigore          #+#    #+#             */
/*   Updated: 2022/01/11 18:10:31 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**init_handler(char **env)
{
	int		i;
	char	**strenv;
	
	i = 0;
	while (env[i])
		i++;
	strenv = malloc(sizeof(char *) * (i + 1));
	if (!strenv)
		return (NULL);
	i = -1;
	while (env[++i])
	{
		strenv[i] = ft_strdup(env[i]);
		if (!strenv[i])
			return (NULL);
	}
	strenv[i] = NULL;
	return (strenv);
}

static char	*other_handler(char **strenv, int opt, char *comp)
{
	int	i;

	i = 0;
	while (strenv[i])
	{
		if (opt == 1)
		{
			if (ft_strncmp(strenv[i], comp, ft_strlen(comp)) == 0)
				return (strenv[i]);
		}
		else if (opt == 2)
		{
			ft_putstr(strenv[i]);
			ft_putchar('\n');
		}
		else if (opt == 3)
			free(strenv[i]);
		i++;
	}
	if (opt == 3)
		free(strenv);
	return (NULL);
}

char	*handler(int opt, char **env, char *comp)
{
	static char	**strenv;
	char	*res;

	if (opt == 0)
	{
		strenv = init_handler(env);
		if (!strenv)
			exit_free(NULL, "Error in init_handler", 0);
	}
	else
	{
		res = other_handler(strenv, opt, comp);
		if (opt == 1)
			return (res);
	}
	return (NULL);
}