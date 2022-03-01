/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 16:31:03 by tamigore          #+#    #+#             */
/*   Updated: 2022/03/01 18:13:36 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*is_here(t_cmd *cmd)
{
	t_token	*token;
	t_token	*ret;

	token = cmd->redir;
	while (token)
	{
		if (token->type == rdin)
		{
			ret = token;
		}	
		else if (token->type == rin)
			ret = NULL;
		token = token->next;
	}
	return (ret);
}

void	close_fd(t_cmd *cmd)
{
	t_token	*token;

	token = cmd->redir;
	while (token)
	{
		if (token->fd != 1 && token->fd != 0)
			close(token->fd);
		token = token->next;
	}
}

int	isntopen(t_cmd *cmd)
{
	t_token	*t;

	t = cmd->redir;
	while (t)
	{
		if ((t->type == rout || t->type == rdout
				|| t->type == rin) && t->fd <= 0)
		{
			if (access(t->next->str, F_OK) != 0 && t->type == rin)
				print_err(t->next->str, ": No such file or directory\n");
			else
				print_err(t->next->str, ": Permission denied\n");
			handler(1, NULL, "?", NULL);
			if (!is_built(cmd))
				exfree(cmd, NULL, 'c', 1);
		}
		t = t->next;
	}
	return (0);
}

void	here_write(int *ftab, t_cmd *c, char *doc)
{
	pipe(ftab);
	c->fdin = ftab[0];
	write(ftab[1], doc, ft_strlen(doc));
	close(ftab[1]);
}

void	fill_fd(t_cmd *c, char *doc)
{
	t_token	*t;
	int		ftab[2];

	t = c->redir;
	while (t)
	{
		if (t->type == rout)
			c->fdout = open(t->next->str, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (t->type == rdout)
			c->fdout = open(t->next->str, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else if (t->type == rin)
		{
			c->fdin = open(t->next->str, O_RDONLY);
			t->fd = c->fdin;
		}
		if (t->type == rout || t->type == rdout)
			t->fd = c->fdout;
		t = t->next;
	}
	if (is_here(c) && doc && !is_built(c))
		here_write(ftab, c, doc);
	if (doc)
		free(doc);
}
