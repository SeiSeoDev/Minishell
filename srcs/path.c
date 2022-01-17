/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 16:07:14 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/17 14:48:54 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ex_cd(t_cmd *cmd)
{
    (void)cmd;
    char *home = NULL;

    if (!cmd->arg->next || (cmd->arg->next->str && !ft_strcmp(cmd->arg->next->str, "~"))) // ICI
        home = handler(7, NULL, "HOME");
	else if ()
	{
		
	}
    printf("HOME :%s\n", home);
    return (0);
}