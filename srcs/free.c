/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 15:34:17 by tamigore          #+#    #+#             */
/*   Updated: 2022/02/23 14:08:39 by dasanter         ###   ########.fr       */
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
	cmd = NULL;
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
	env = NULL;
}

void	ctrfree(void *ptr, char *err, char type, int num)
{
	if (type == 't')
		free_token(ptr);
	else if (type == 'c')
		free_cmd(ptr);
	if (err)
		printf("%s\n", err);
	errno = num;
	ptr = NULL;
}

void	exfree(void *ptr, char *err, char type, int num)
{
	handler(4, NULL, NULL, NULL);
	if (type == 't')
		free_token(ptr);
	else if (type == 'c')
		free_cmd(ptr);
	if (err)
		printf("%s\n", err);
	exit(num);
}
