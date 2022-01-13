/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 17:02:46 by tamigore          #+#    #+#             */
/*   Updated: 2022/01/12 18:58:02 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*init_handler(char **env)
{
	int		i;
	t_env	*myenv;
	t_env	*tmp;
	
	if (!env)
		return (NULL);
	myenv = init_env(NULL, env[0]);
	if (!myenv)
		return (NULL);
	i = 0;
	tmp = myenv;
	while (env[++i])
	{
		tmp->next = init_env(NULL, env[i]);
		if (!tmp)
		{
			free_env(myenv);
			return (NULL);
		}
		tmp = tmp->next;
	}
	return (myenv);
}

static char	*other_handler(t_env *myenv, int opt, char *comp)
{
	t_env	*tmp;

	tmp = NULL;
	if (opt == 3)
		free_env(myenv);
	else
		tmp = myenv;
	while (tmp)
	{
		if (opt == 1)
		{
			if (ft_strncmp(tmp->str, comp, ft_strlen(comp)) == 0)
				return (tmp->str);
		}
		else if (opt == 2)
		{
			ft_putstr(tmp->str);
			ft_putchar('\n');
		}
		tmp = tmp->next;
	}
	return (NULL);
}

static void	delone_env(t_env *env, char *del)
{
	t_env *tmp;
	t_env *save;

	tmp = env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->str, del, ft_strlen(del)))
		{
			save->next = tmp->next;
			tmp->next = NULL;
			free_env(tmp);
		}
		save = tmp;
		tmp = tmp->next;
	}
}

static void	addone_env(t_env *env, char *new)
{
	t_env *tmp;

	tmp = env;
	while (tmp)
		tmp = tmp->next;
	tmp = init_env(NULL, new);
	if (!tmp)
		exit_free(NULL, "Error addone_env\n", 0);
}

char	*handler(int opt, char **env, char *comp)
{
	static t_env	*myenv;
	char			*res;

	if (opt == 0)
	{
		myenv = init_handler(env);
		if (!myenv)
			exit_free(NULL, "Error in init_handler", 0);
	}
	else if (opt == 1 || opt == 2 || opt == 3)
	{
		res = other_handler(myenv, opt, comp);
		if (opt == 1)
			return (res);
	}
	else
	{
		if (opt == 5)
			delone_env(myenv, comp);
		else if (opt == 4)
			addone_env(myenv, comp);
	}
	return (NULL);
}
