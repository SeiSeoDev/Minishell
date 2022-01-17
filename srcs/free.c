/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 15:34:17 by tamigore          #+#    #+#             */
/*   Updated: 2022/01/17 14:57:31 by dasanter         ###   ########.fr       */
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

void	free_env(t_env *env)
{
	t_env	*tmp;

	if (env)
	{
		while (env)
		{
			if (env->str)
				free(env->str);
			tmp = env;
			env = env->next;
			free(tmp);
		}
	}
}

void	exit_free(void *ptr, char *err, char type)
{
	if (type == 't')
		free_token(ptr);
	else if (type == 'c')
		free_cmd(ptr);
	printf("%s\n", err);
	exit(1);
}