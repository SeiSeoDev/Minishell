/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 16:28:32 by tamigore          #+#    #+#             */
/*   Updated: 2022/02/27 17:08:52 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_add(t_token **tmp, t_cmd *data)
{
	t_token	*use;

	if ((*tmp)->type != word && !data->redir)
		data->redir = cmd_redir(tmp);
	else if ((*tmp)->type != word && data->redir)
	{
		use = data->redir;
		while (use->next)
			use = use->next;
		use->next = cmd_redir(tmp);
	}
	else if ((*tmp)->type == word && !data->arg)
		data->arg = cmd_arg(tmp);
	else if ((*tmp)->type == word && !data->arg)
	{
		use = data->arg;
		while (use->next)
			use = use->next;
		use->next = cmd_arg(tmp);
	}
}

t_cmd	*cmd_init(t_cmd *res, t_token **tmp, t_token *token)
{
	t_token	*use;
	t_cmd	*data;

	data = init_cmd(NULL, NULL, NULL);
	if (!data)
	{
		free_token(token);
		ctrfree(res, "init token failled\n", 'c', 1);
		return (NULL);
	}
	use = *tmp;
	*tmp = (*tmp)->next;
	use->next = NULL;
	free_token(use);
	return (data);
}

t_cmd	*parsing_error(t_cmd *cmd)
{
	t_token	*red;

	if (cmd && cmd->redir)
	{
		red = cmd->redir;
		while (red)
		{
			if ((red->type == word || red->type == pip)
				|| ((red->type == rdout || red->type == rdin
						|| red->type == rin || red->type == rout)
					&& red->next && red->next->type >= 3)
				|| ((red->type == rdout || red->type == rdin
						|| red->type == rin || red->type == rout)
					&& (!red->next)))
			{
				printf("Minishell: syntax error near unexpected token: %s\n",
					red->str);
				ctrfree(cmd, NULL, 'c', 2);
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
