/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_built.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 16:07:14 by dasanter          #+#    #+#             */
/*   Updated: 2022/03/02 14:37:12 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	onlyn(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	ex_echo(t_cmd *cmd)
{
	int		n;
	t_token	*arg;

	arg = cmd->arg->next;
	n = 0;
	while (arg)
	{
		if (arg && arg->str && !ft_strncmp(arg->str, "-n", 2)
			&& onlyn(&arg->str[1]))
			n = 1;
		else
		{
			ft_putstr_fd(arg->str, cmd->fdout);
			if (arg->next)
				ft_putstr_fd(" ", cmd->fdout);
		}
		arg = arg->next;
	}
	if (!n)
		write(cmd->fdout, "\n", 1);
}

void	ex_cd(t_cmd *cmd)
{
	char	buf[4096];
	char	*str;

	str = NULL;
	if (cmd->arg->next == NULL)
		str = ft_strdup(handler(3, NULL, "HOME", NULL)->val);
	else
		str = cmd->arg->next->str;
	if (!ft_strcmp(str, "~"))
		str = handler(3, NULL, "HOME", NULL)->val;
	else if (str[0] == '~' && str[1])
	{
		str = ft_strjoin(handler(3, NULL, "HOME", NULL)->val, "/");
		str = ft_strjoin(str, &cmd->arg->next->str[1]);
	}
	if (chdir(str) == -1)
		printf("Minishell: cd: %s: Not a directory\n", str);
	else
	{
		handler(3, NULL, "OLDPWD", handler(3, NULL, "PWD", NULL)->val);
		handler(3, NULL, "PWD", getcwd(buf, 4096));
	}
}

void	ex_pwd(t_cmd *cmd)
{
	t_env	*myenv;

	(void)cmd;
	myenv = handler(3, NULL, "PWD", NULL);
	write(1, myenv->val, ft_strlen(myenv->val));
	write(1, "\n", 1);
}

int	is_built(t_cmd *cmd)
{
	if (!cmd || !cmd->arg || !cmd->arg->str)
		return (0);
	if (!ft_strcmp(cmd->arg->str, "echo"))
		return (1);
	else if (!ft_strcmp(cmd->arg->str, "cd"))
		return (2);
	else if (!ft_strcmp(cmd->arg->str, "pwd"))
		return (3);
	else if (!ft_strcmp(cmd->arg->str, "env"))
		return (4);
	else if (!ft_strcmp(cmd->arg->str, "unset"))
		return (5);
	else if (!ft_strcmp(cmd->arg->str, "export"))
		return (6);
	else if (!ft_strcmp(cmd->arg->str, "exit"))
		return (7);
	return (0);
}
