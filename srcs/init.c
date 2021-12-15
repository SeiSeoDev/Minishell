/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:17:14 by tamigore          #+#    #+#             */
/*   Updated: 2021/12/15 18:09:54 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*init_token(t_token *next, char *str, int type)
{
	t_token	*res;

	res = (t_token *)malloc(sizeof(t_token));
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
	data->fdout = 0;
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
