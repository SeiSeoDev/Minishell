/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 17:02:46 by tamigore          #+#    #+#             */
/*   Updated: 2022/01/19 17:13:12 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*init_handler(char **env)
{
	int		i;
	t_env	*myenv;
	t_env	*tmp;
	
	myenv = init_env(NULL, get_name(env[0]), get_value(env[0]));
	if (!myenv)
		return (NULL);
	i = 0;
	tmp = myenv;
	while (env[i])
	{
		tmp->next = init_env(NULL, get_name(env[i]), get_value(env[i]));
		if (!tmp->next)
			return (NULL);
		tmp = tmp->next;
		i++;
	}
	return (myenv);
}

static t_env	*delone_env(t_env *env, char *del)
{
	t_env	*tmp;
	t_env	*save;

	tmp = env;
	save = NULL;
	while (tmp)
	{
		if (!ft_strncmp(tmp->name, del, ft_strlen(del)))
		{
			save->next = tmp->next;
			tmp->next = NULL;
			free_env(tmp);
			tmp = save;
			break ;
		}
		save = tmp;
		tmp = tmp->next;
	}
	if (!tmp)
		save = NULL;
	return (save);
}

static t_env	*addone_env(t_env *env, char *name, char *val)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
		tmp = tmp->next;
	tmp = init_env(NULL, ft_strdup(name), ft_strdup(val));
	if (!tmp)
		exit_free(NULL, "Error addone_env\n", 0);
	return (tmp);
}

static t_env	*mod_env(t_env *env, char *name, char *val)
{
	t_env	*tmp;

	tmp = env;
	if (name && val)
	{
		while (tmp)
		{
			if (!ft_strncmp(name, tmp->name, ft_strlen(name)))
			{
				free(tmp->val);
				tmp->val = ft_strdup(val);
				break ;
			}
			tmp = tmp->next;
		}
	}
	else if (name)
	{
		while (tmp)
		{
			if (!ft_strncmp(name, tmp->name, ft_strlen(name)))
				break ;
			tmp = tmp->next;
		}
	}
	return (tmp);
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
		res = addone_env(myenv, name, val);
	else if (opt == 2)
		res = delone_env(myenv, name);
	else if (opt == 3)
		res = mod_env(myenv, name, val);
	else if (opt == 4)
		free_env(myenv);
	return (res);
}
