/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_built.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 16:07:14 by dasanter          #+#    #+#             */
/*   Updated: 2022/03/02 17:20:15 by tamigore         ###   ########.fr       */
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
	t_env	*env;

	str = NULL;
	if (cmd->arg->next)
		str = cmd->arg->next->str;
	if (!cmd->arg->next || !ft_strcmp(str, "~"))
		str = ft_strdup("/mnt/nfs/homes/tamigore");
	if (str[0] == '~' && str[1])
	{
		str = ft_strjoin("/mnt/nfs/homes/tamigore", "/");
		str = ft_strjoin(str, &cmd->arg->next->str[1]);
	}
	if (chdir(str) == -1)
		printf("Minishell: cd: %s: Not a directory\n", str);
	else
	{
		env = handler(3, NULL, "PWD", NULL);
		if (env)
			handler(3, NULL, "OLDPWD", env->val);
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
