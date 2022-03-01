/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 16:31:03 by tamigore          #+#    #+#             */
/*   Updated: 2022/03/01 17:17:29 by tamigore         ###   ########.fr       */
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

void	print_err(char *str, char *file)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(file, 2);
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

void	fill_fd(t_cmd *c, char *doc)
{
	t_token	*t;
	int		ftab[2];
	int		her;

	t = c->redir;
	her = 0;
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
			her = 0;
		}
		else if (t->type == rdin)
			her = 1;
		if (t->type == rout || t->type == rdout)
			t->fd = c->fdout;
		t = t->next;
	}
	if (her && doc && !is_built(c))
	{
		pipe(ftab);
		c->fdin = ftab[0];
		write(ftab[1], doc, ft_strlen(doc));
		close(ftab[1]);
	}
	if (doc)
		free(doc);
}
