/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 17:02:46 by tamigore          #+#    #+#             */
/*   Updated: 2022/02/17 11:00:02 by user42           ###   ########.fr       */
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
	while (env[++i])
	{
		tmp->next = init_env(NULL, get_name(env[i]), get_value(env[i]));
		if (!tmp->next)
			return (NULL);
		tmp = tmp->next;
	}
	return (myenv);
}

static t_env	*delone_env(t_env **env, char *del)
{
	t_env	*tmp;
	t_env	*save;

	tmp = *env;
	save = NULL;
	if (*env && (!ft_strncmp((*env)->name, del, ft_strlen(del))))
	{
		*env = (*env)->next;
		tmp->next = NULL;
		free_env(tmp);
		return (*env);
	}
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
		exfree(NULL, "Error addone_env\n", 0, 1);
	return (tmp);
}

static t_env	*mod_env(t_env *env, char *name, char *val, int add)
{
	t_env	*tmp;

	tmp = env;
	if (!name)
		return (tmp);
	while (tmp)
	{
		if (!ft_strcmp(name, tmp->name))
		{
			if (add == 1 && val)
				tmp->val = ft_free_join(tmp->val, val, 1);
			else if (add == 0 && val)
			{
				free(tmp->val);
				tmp->val = ft_strdup(val);
			}
			return (tmp);
		}
		if (!tmp->next)
		{
			if (name && val)
				tmp->next = init_env(NULL, ft_strdup(name), ft_strdup(val));
			return (tmp->next);
		}
		tmp = tmp->next;
	}
	if (!tmp)
	{
		tmp = init_env(NULL, ft_strdup(name), ft_strdup(val));
		printf("LOL\n");
	}
	return (tmp);
}

t_env	*handler(int opt, char **env, char *name, char *val)
{
	t_env			*res;
	static t_env	*myenv;

	res = NULL;
	if (opt == 0)
	{
		myenv = init_handler(env);
		if (!myenv)
			exfree(NULL, "Error in init_handler", 0, 1);
	}
	else if (opt == 1)
		res = addone_env(myenv, name, val);
	else if (opt == 2)
		res = delone_env(&myenv, name);
	else if (opt == 3)
		res = mod_env(myenv, name, val, 0);
	else if (opt == 4)
		free_env(myenv);
	else if (opt == 5)
		res = mod_env(myenv, name, val, 1);
	return (res);
}
