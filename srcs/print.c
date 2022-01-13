/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 13:44:02 by tamigore          #+#    #+#             */
/*   Updated: 2022/01/13 16:51:36 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_token(t_token *token)
{
	int		i;
	t_token	*tmp;

	if (token)
	{
		i = 0;
		tmp = token;
		printf("Start print token\n");
		while (tmp)
		{
			printf("token [%d]: TYPE[%d] word=%s\n", i++, tmp->type, tmp->str);
			if (tmp->next)
				tmp = tmp->next;
			else
				break ;
		}
	}
	else
		printf("No token...\n");
}

void	print_cmd(t_cmd *cmd)
{
	int		i;
	t_cmd	*tmp;

	if (cmd)
	{
		i = 0;
		tmp = cmd;
		while (tmp)
		{
			printf("cmd [%d]:\n", i++);
			if (cmd->arg)
				print_token(tmp->arg);
			else
				printf("no arg...\n");
			if (cmd->redir)
				print_token(tmp->redir);
			else
				printf("no redir...\n");
			printf("fdin = %d | fd out = %d | pid = %d\n", tmp->fdin, tmp->fdout, tmp->pid);
			tmp = tmp->next;
		}
	}
	else
		printf("No comand...\n");
}
