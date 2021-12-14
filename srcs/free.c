/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 15:34:17 by tamigore          #+#    #+#             */
/*   Updated: 2021/12/14 14:39:07 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_token *token)
{
	t_token	*tmp;

	if (token)
	{
		while (token)
		{
			if (token->str)
				free(token->str);
			tmp = token;
			token = token->next;
			free(tmp);
		}
	}
}

void	free_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;

	if (cmd)
	{
		while (cmd)
		{
			if (cmd->arg)
				free_token(cmd->arg);
			if (cmd->redir)
				free_token(cmd->redir);
			tmp = cmd;
			cmd = cmd->next;
			free(tmp);
		}
	}
}

void	exit_free(t_cmd *cmd, char *err)
{
	if (cmd)
		free_cmd(cmd);
	printf("%s\n", err);
}