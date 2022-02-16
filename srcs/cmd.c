/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 16:28:32 by tamigore          #+#    #+#             */
/*   Updated: 2022/02/16 12:03:35 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parsing_error(t_cmd *cmd)
{
	t_token	*red;

	red = cmd->redir;
	if (red)
	{
		while (red)
		{
			if (red->type == word || red->type == pip)
			{
				ctrfree(cmd, "syntax error near unexpected token\n", 'c', 1);
				return (NULL);
			}
			else if (red->type > 3 && (!red->next || red->next->type > 3))
			{
				ctrfree(cmd, "syntax error near unexpected token\n", 'c', 1);
				return (NULL);
			}
			else if (red->type == rdout && (!red->next))
			{
				ctrfree(cmd, "syntax error near unexpected token\n", 'c', 1);
				return (NULL);
			}
			red = red->next;
		}
	}
	return (cmd);
}

t_token	*cmd_arg(t_token **tmp)
{
	t_token	*res;
	t_token	*stop;

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
	t_token	*res;
	t_token	*stop;

	res = *tmp;
	if (*tmp)
	{
		if ((*tmp)->type == pip)
			return (NULL);
		while ((*tmp)->next && (*tmp)->next->type != word
			&& (*tmp)->next->type != pip)
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
