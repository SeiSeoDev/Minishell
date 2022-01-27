/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 16:28:32 by tamigore          #+#    #+#             */
/*   Updated: 2022/01/27 16:55:53 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parsing_error(t_cmd *cmd)
{
	t_cmd   *tmp;
	
	tmp = cmd;
	if (tmp)
	{
		while (tmp->redir)
		{
			if (tmp->redir->type == word || tmp->redir->type == pip)
				exit_free(cmd, "minishell: syntax error near unexpected token\n", 'c');
			else if (tmp->redir->type > 3 && (!tmp->redir->next ||  tmp->redir->next->type > 3))
				exit_free(cmd, "minishell: syntax error near unexpected token\n", 'c');
			else if (tmp->redir->type == rdout && (!tmp->redir->next || tmp->redir->next->type != lim))
				exit_free(cmd, "minishell: syntax error near unexpected token\n", 'c');
		}
	}
}

t_token	*cmd_arg(t_token **tmp)
{
	t_token *res;
	t_token *stop;

	res = *tmp;
	if (*tmp)
	{
		if ((*tmp)->type == pip)
			return (NULL);
		while ((*tmp)->next && (*tmp)->next->type == word)
			(*tmp) = (*tmp)->next;
	}
	stop = res;
	while (stop != *tmp)
		stop = stop->next;
	if (*tmp)
		*tmp = (*tmp)->next;
	if (stop)
		stop->next = NULL;
	return (res);
}

t_token	*cmd_redir(t_token **tmp)
{
	t_token *res;
	t_token *stop;

	res = *tmp;
	if (*tmp)
	{
		if ((*tmp)->type == pip)
			return (NULL);
		while ((*tmp)->next && (*tmp)->next->type != word && (*tmp)->next->type != pip)
			(*tmp) = (*tmp)->next;
	}
	stop = res;
	while (stop != *tmp)
		stop = stop->next;
	if (*tmp)
		*tmp = (*tmp)->next;
	if (stop)
		stop->next = NULL;
	return (res);
}