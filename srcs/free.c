/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 15:34:17 by tamigore          #+#    #+#             */
/*   Updated: 2022/01/20 16:49:55 by tamigore         ###   ########.fr       */
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
			{
				free(token->str);
				printf(".");
			}
			tmp = token;
			token = token->next;
			free(tmp);
			printf("token free\n");
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

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

void	exit_free(void *ptr, char *err, char type)
{
	handler(4, NULL, NULL, NULL);
	if (type == 't')
		free_token(ptr);
	else if (type == 'c')
		free_cmd(ptr);
	printf("%s\n", err);
	exit(1);
}