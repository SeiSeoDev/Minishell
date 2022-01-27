/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 13:39:02 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/27 19:13:04 by dasanter         ###   ########.fr       */
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
    if (arg && !ft_strcmp(arg->str, "-n"))
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

int		get_equalpos(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		else if (str[i] == '+' && str[i + 1] == '=')
			return (2);
		if ((str[i] >= 0 && str[i] < 42) || (str[i] >= 123 && str[i] <= 127) ||
				(str[i] >= 44 && str[i] <= 47) || (str[i] >= 58 && str[i] <= 60) ||
				(str[i] >= 62 && str[i] <= 64) || (str[i] >= 91 && str[i] <= 96))
			return (0);
		i++;
	}
	return (0);
}

char	**get_separation(char *str)
{
	static char	*tab[2];
	char		*tmp;
	int			i;

	tmp = str;
	i = 0;
	tab[0] = &tmp[i];
	while (tmp[i] && tmp[i] != '=' && tmp[i] != '+')
		i++;
	if (tmp[i] == '+' && tmp[i + 1] == '=')
	{
		tmp[i++] = 0;
		tmp[i++] = 0;
	}
	else if (tmp[i] == '=')
		tmp[i++] = 0;
	tab[1] = &tmp[i];
	return (tab);
}

void	ex_port(t_cmd *cmd)
{
	t_token	*arg;
	char	**tab;

	if (cmd->arg->next)
		arg = cmd->arg->next;
	else
	{
		ex_env(cmd);
		return;
	}
	while (arg)
	{
		if (arg->str && get_equalpos(arg->str) == 1)
		{
			tab = get_separation(arg->str);
			// printf("TEST : |%s|\n", tab[0]);
			// printf("TEST : |%s|\n", tab[1]);
			handler(3, NULL, tab[0], tab[1]);
		}
		else if (arg->str && get_equalpos(arg->str) == 2)
		{
			tab = get_separation(arg->str);
			// printf("TEST : |%s|\n", tab[0]);
			// printf("TEST : |%s|\n", tab[1]);
			handler(5, NULL, tab[0], tab[1]);
		}
		else
			exit_free(cmd, "minishell: export: identifiant non valable\n", 'c');
		arg = arg->next;
	}
	return;
}

void    ex_unset(t_cmd *cmd)
{
	t_token	*tmp;

	if (!cmd->arg->next)
		ft_putstr_fd("unset: not enough arguments\n", 2);
	tmp = cmd->arg->next;
	while (tmp)
	{
		handler(2, NULL, tmp->str, NULL);
		tmp = tmp->next;
	}
}
