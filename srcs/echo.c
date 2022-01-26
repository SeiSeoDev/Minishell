/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 13:39:02 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/26 16:44:46 by user42           ###   ########.fr       */
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

    env = get_env(handler(3, NULL, NULL, NULL));
    i = 0;
    while (env[i])
    {
        ft_putstr_fd(env[i], cmd->fdout);
        write(cmd->fdout, "\n", 1);
        i++;
    }
    free(env);
}

int     get_equalpos(char *str)
{
    int i;
    int ret;

    ret = 0;
    i = 0;
    while (str[i])
    {
        if (str[i] == '=')
        {
            ret = i;
            return (ret);
        }
        i++;
    }
    return (ret);
}

char **get_separation(char *str)
{
    static char *tab[2];
    char *tmp;

    tmp = str;
    tab[0] = str;
    while (*tmp != '=')
        tmp++;
    *tmp = 0;
    tmp++;
    tab[1] = tmp;
    return (tab);
}

void    ex_port(t_cmd *cmd)
{
    char *arg;
    char **tab;

    if (cmd->arg->next)
        arg=cmd->arg->next->str;
    else
    {
        ex_env(cmd);
        return;
    }
    if (arg && get_equalpos(arg))
    {
        tab = get_separation(arg);
        // printf("TEST : |%s|\n", tab[0]);
        // printf("TEST : |%s|\n", tab[1]);
        handler(3, NULL, tab[0], tab[1]);
    }
    return;
}

void    ex_unset(t_cmd *cmd)
{
    t_token *tmp;
    if (!cmd->arg->next)
        ft_putstr_fd("unset: not enough arguments\n", 2);
    tmp = cmd->arg->next;
    while (tmp)
    {
        handler(2, NULL, tmp->str, NULL);
        tmp = tmp->next;
    }
}
