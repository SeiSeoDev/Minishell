/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 15:34:17 by tamigore          #+#    #+#             */
/*   Updated: 2021/12/08 15:39:57 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_arg(t_arg *arg)
{
	if (arg->cmd)
		free(arg->cmd);
	if (arg->opt)
		free(arg->opt);
	if (arg->oper)
		free(arg->oper);
	free(arg);
}

void	exit_free(t_pars *pars, char *err)
{
	t_pipe	*tmp;

	if (pars)
	{
		if (pars->pipe)
		{
			while (pars->pipe)
			{
				free_arg(pars->pipe->arg);
				tmp = pars->pipe;
				pars->pipe = pars->pipe->next;
				free(tmp);
			}
		}
		if (pars->arg)
			free_arg(pars->arg);
	}
	printf("%s\n", err);
}