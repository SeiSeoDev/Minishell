/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:17:14 by tamigore          #+#    #+#             */
/*   Updated: 2021/12/08 16:18:09 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pipe(t_pars *pars)
{
	(void)pars;
}

void	init_arg(t_pars *pars)
{
	int	i;
	pars->arg = malloc(sizeof(t_arg));
	if (!pars->arg)
		exit_free(pars, "malloc error");
	pars->arg->str = pars->line;
	pars->arg->cmd = NULL;
	pars->arg->oper = NULL;
	pars->arg->opt = NULL;
	i = get_cmd(pars->arg->cmd, pars->line);
	i = get_oper(pars->arg->oper, pars->line, i);
	i = get_opt(pars->arg->opt, pars->line, i);
}
