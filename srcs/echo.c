/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 13:39:02 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/12 15:08:28 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int test_error(t_cmd *cmd)
{
    return (0);
}

int     ex_echo(t_cmd *cmd)
{
    int n;
    int fd;

    if (!redir)
        fd = 1;
    
    t_token *arg;

    arg = cmd->arg->next;
    n = 0;
    if (test_error())
        return (-1);
    if (!ft_strcmp(arg->str, "-n"))
    {
        n = 1;
        arg = arg->next;
    }
    while (arg)
    {
        ft_putstr(arg->str);
        arg = arg->next;
    }
    if (!n)
        write(fd, "\n", 1);
}