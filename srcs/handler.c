/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 17:02:46 by tamigore          #+#    #+#             */
/*   Updated: 2022/01/19 16:40:59 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*init_handler(char **env)
{
	int		i;
	t_env	*myenv;
	t_env	*tmp;
	char	**all;
	
	all = get_env(env);
	if (!all)
		return (NULL);
	myenv = init_env(NULL, get_name(env[0]), get_value(env[0]), all);
	if (!myenv)
		return (NULL);
	i = 1;
	tmp = myenv;
	while (env[i])
	{
		tmp->next = init_env(NULL, get_name(env[i]), get_value(env[i]), all);
		if (!tmp->next)
			return (NULL);
		tmp = tmp->next;
		i++;
	}
	return (myenv);
}

static void	delone_env(t_env *env, char *del)
{
	t_env	*tmp;
	t_env	*save;
	int		x;

	tmp = env;
	x = 0;
	while (tmp)
	{
		if (!ft_strncmp(tmp->name, del, ft_strlen(del)))
		{
			save->next = tmp->next;
			tmp->next = NULL;
			if (x > 0)
				tmp->all = NULL;
			free_env(tmp);
		}
		x++;
		save = tmp;
		tmp = tmp->next;
	}
}

static void	addone_env(t_env *env, char *name, char *val)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
		tmp = tmp->next;
	tmp = init_env(NULL, ft_strdup(name), ft_strdup(val), env->all);
	if (!tmp)
		exit_free(NULL, "Error addone_env\n", 0);
}

static t_env	*mod_env(t_env *env, char *name, char *val)
{
	t_env	*tmp;
	char	*tofree;

	tmp = env;
	if (!name && !val)
		return (env);
	else if (name && val)
	{
		while (tmp)
		{
			if (!ft_strncmp(name, tmp->name, ft_strlen(name)))
			{
				tofree = tmp->val;
				tmp->val = val;
				free(tofree);
			}
			tmp = tmp->next;
		}
	}
	else if (name)
	{
		while (tmp)
		{
			if (!ft_strncmp(name, tmp->name, ft_strlen(name)))
				return (tmp);
			tmp = tmp->next;
		}
	}
	return (NULL);
}

t_env	*handler(int opt, char **env, char *name, char *val)
{
	static t_env	*myenv;
	t_env			*res;

	res = NULL;
	if (opt == 0)
	{
		myenv = init_handler(env);
		if (!myenv)
			exit_free(NULL, "Error in init_handler", 0);
	}
	else if (opt == 1)
		addone_env(myenv, name, val);
	else if (opt == 2)
		delone_env(myenv, name);
	else if (opt == 3)
		res = mod_env(myenv, name, val);
	else if (opt == 4)
		free_env(myenv);
	return (res);
}
