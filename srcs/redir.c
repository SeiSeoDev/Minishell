/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 16:31:03 by tamigore          #+#    #+#             */
/*   Updated: 2022/03/01 00:03:07 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			if (access(t->next->str, F_OK) != 0)
				printf("Minishell: %s: No such file or directory\n",
					t->next->str);
			else
				printf("Minishell: %s: Permission denied\n", t->next->str);
			exfree(cmd, NULL, 'c', 1);
		}
		t = t->next;
	}
	return (0);
}

char	*fill_fd(t_cmd *c, char *doc)
{
	t_token	*t;

	t = c->redir;
	while (t)
	{
		if (doc && (t->type == rdin || t->type == rin))
			free(doc);
		if (t->type == rout)
			c->fdout = open(t->next->str, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (t->type == rdout)
			c->fdout = open(t->next->str, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else if (t->type == rin)
		{
			c->fdin = open(t->next->str, O_RDONLY);
			t->fd = c->fdin;
		}
		else if (t->type == rdin)
			doc = heredoc(t->next);
		if (t->type == rout || t->type == rdout)
			t->fd = c->fdout;
		t = t->next;
	}
	return (doc);
}
/*
**	if (doc)
**	{
**		if (pipe(pipfd) == -1)
**			return (NULL);
**		write(pipfd[1], doc, ft_strlen(doc));
**		dup2(pipfd[0], cmd->fdin);
**		close(pipfd[1]);
**		close(pipfd[0]);
**		free(doc);
**	}
*/