/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 15:50:00 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/17 16:19:19 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    exec(t_cmd *cmd)
{
    //printf("first\n");
    if (cmd != NULL && cmd->arg != NULL)
    {
        //printf("TEST : %s\n", cmd->arg->str);
        if (!ft_strcmp(cmd->arg->str, "echo"))
            ex_echo(cmd);
        if (!ft_strcmp(cmd->arg->str, "cd"))
            ex_cd(cmd);
        if (!ft_strcmp(cmd->arg->str, "pwd"))
            ex_pwd(cmd);
    }
}