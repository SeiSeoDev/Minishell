/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 15:50:00 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/13 19:00:05 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec(t_cmd *cmd)
{
	int	cpid;

	printf("Exec Start\n");
	if (cmd)
	{
		cpid = fork();
		if (cpid == 0)
		{
			printf("start exec command\n");
			exit(cpid);
		}
		else
		{
			printf("next command : %d\n", cpid);
		}
	}
	else
	{
		printf("\n");
		//prompt;
	}
}