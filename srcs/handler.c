/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 17:02:46 by tamigore          #+#    #+#             */
/*   Updated: 2022/02/26 16:11:50 by user42           ###   ########.fr       */
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
		exfree(NULL, "Error init_handler", 0, 1);
	i = 0;
	tmp = myenv;
	while (env[++i])
	{
		tmp->next = init_env(NULL, get_name(env[i]), get_value(env[i]));
		if (!tmp->next)
		{
			while (myenv)
			{
				tmp = myenv;
				myenv = myenv->next;
				free(tmp);
			}
			exfree(NULL, "Error init_handler", 0, 1);
		}
		tmp = tmp->next;
	}
	return (myenv);
}

static t_env	*delone_env(t_env **env, char *del, t_env *save)
{
	t_env	*tmp;

	tmp = *env;
	if (*env && del && (!ft_strncmp((*env)->name, del, ft_strlen(del))))
	{
		*env = (*env)->next;
		tmp->next = NULL;
		free_env(tmp);
		return (*env);
	}
	while (tmp)
	{
		if (del && !ft_strncmp(tmp->name, del, ft_strlen(del)))
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
	if (!name)
		return (env);
	while (tmp)
		tmp = tmp->next;
	tmp = init_env(NULL, ft_strdup(name), ft_strdup(val));
	if (!tmp)
		exfree(NULL, "Error addone_env\n", 0, 1);
	return (tmp);
}

static t_env	*mod_env(t_env **env, char *n, char *v, int add)
{
	t_env	*t;

	t = *env;
	while (t && n)
	{
		if (!ft_strcmp(n, t->name))
		{
			if (add == 5 && v)
				t->val = ft_free_join(t->val, v, 1);
			else if (add == 3 && v)
			{
				free(t->val);
				t->val = ft_strdup(v);
			}
			return (t);
		}
		if (!t->next)
			if (n && v)
				return (t->next = init_env(NULL, ft_strdup(n), ft_strdup(v)));
		t = t->next;
	}
	if (!t && n && v)
		return (*env = init_env(NULL, ft_strdup(n), ft_strdup(v)));
	return (t);
}

t_env	*handler(int opt, char **env, char *name, char *val)
{
	t_env			*res;
	static t_env	*myenv;
	static int		exit_status;

	res = NULL;
	if (name && !ft_strcmp(name, "?"))
	{
		exit_status = opt;
		return (NULL);
	}
	if (val && !ft_strcmp(val, "?"))
		return (init_env(NULL, NULL, ft_itoa(exit_status)));
	if (opt == 0 && env)
	{
		exit_status = 0;
		myenv = init_handler(env);
	}
	else if (opt == 1)
		res = addone_env(myenv, name, val);
	else if (opt == 2)
		res = delone_env(&myenv, name, NULL);
	else if (opt == 3)
		res = mod_env(&myenv, name, val, opt);
	else if (opt == 4)
		free_env(myenv);
	return (res);
}
