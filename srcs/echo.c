/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 13:39:02 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/19 14:26:35 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int test_error(t_cmd *cmd)
{
    (void)cmd;
    return (0);
}

int     ex_echo(t_cmd *cmd)
{
    int n;
    t_token *arg;

    arg = cmd->arg->next;
    n = 0;
    if (test_error(cmd))
        return (-1);
    if (!ft_strcmp(arg->str, "-n"))
    {
        n = 1;
        arg = arg->next;
    }
    while (arg)
    {
        ft_putstr_fd(arg->str, cmd->fdout);
        arg = arg->next;
        if (arg)
            ft_putstr_fd(" ", cmd->fdout);
    }
    if (!n)
        write(cmd->fdout, "\n", 1);
    return (1);
}

void    ex_env(t_cmd *cmd)
{
    t_env *env;
    int i;

    env = handler(5, NULL, NULL, NULL);
    i = 0;
    while (env->all[i])
    {
        ft_putstr_fd(env->all[i], cmd->fdout);
        write(cmd->fdout, "\n", 1);
        i++;
    }
}

void    ex_port(t_cmd *cmd)
{
    (void)cmd;
}

void    ex_unset(t_cmd *cmd)
{
    if (!cmd->arg->next)
        ft_putstr_fd("unset: not enough arguments\n", 2);
    handler(2, NULL, cmd->arg->next->str, NULL);
}