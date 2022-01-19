/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 13:39:02 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/19 16:40:44 by dasanter         ###   ########.fr       */
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
    char **env;
    int i;

    env = handler(3, NULL, NULL, NULL)->all;
    i = 0;
    while (env[i])
    {
        ft_putstr_fd(env[i], cmd->fdout);
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