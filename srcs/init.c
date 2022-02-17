/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:17:14 by tamigore          #+#    #+#             */
/*   Updated: 2022/02/17 10:53:49 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*init_env(t_env *next, char *name, char *val)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->next = next;
	env->name = name;
	env->val = val;
	return (env);
}

t_token	*init_token(t_token *next, char *str, int type)
{
	t_token	*res;

	res = malloc(sizeof(t_token));
	if (!res)
		return (NULL);
	res->type = type;
	res->str = str;
	res->next = next;
	return (res);
}

t_cmd	*init_cmd(t_cmd *next, t_token *arg, t_token *redir)
{
	t_cmd	*data;

	data = malloc(sizeof(t_cmd));
	if (!data)
		return (NULL);
	data->arg = arg;
	data->next = next;
	data->redir = redir;
	data->fdin = 0;
	data->fdout = 1;
	data->pid = 0;
	return (data);
}

void	add_token(t_token *token, t_token *new)
{
	t_token *tmp;

	tmp = token->next;
	token->next = new;
	new->next = tmp;
}

void	del_token(t_token **token, t_token *del)
{
	t_token *tmp;

	if (*token == del)
	{
		*token = (*token)->next;
		free_token(del);
	}
	tmp = *token;
	while (tmp)
	{
		if (tmp->next == del)
		{
			tmp->next = tmp->next->next;
			free_token(del);
			return ;
		}
		tmp = tmp->next;
	}
}
