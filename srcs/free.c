/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 15:34:17 by tamigore          #+#    #+#             */
/*   Updated: 2022/01/19 17:09:44 by tamigore         ###   ########.fr       */
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

void	free_env(t_env *env)
{
	t_env	*tmp;

	if (!env)
		return ;
	while (env)
	{
		if (env->name)
			free(env->name);
		if (env->val)
			free(env->val);
		tmp = env;
		env = env->next;
		free(tmp);
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

void	free_all(t_cmd *cmd)
{
	handler(4, NULL, NULL, NULL);
	if (cmd)
		free_cmd(cmd);
	printf("exit success\n");
	exit(EXIT_SUCCESS);
}