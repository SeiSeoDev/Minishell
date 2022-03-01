/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_built.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 16:07:14 by dasanter          #+#    #+#             */
/*   Updated: 2022/03/01 17:39:47 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ex_echo(t_cmd *cmd)
{
	int		n;
	t_token	*arg;

	arg = cmd->arg->next;
	n = 0;
	if (arg && arg->str && !ft_strcmp(arg->str, "-n"))
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
	printf("str : %s\n", str);
	if (!ft_strcmp(str, "~"))
		str = handler(3, NULL, "HOME", NULL)->val;
	else if (str[0] == '~' && str[1])
	{
		str = ft_strjoin(handler(3, NULL, "HOME", NULL)->val, "/");
		str = ft_strjoin(str, cmd->arg->next->str);
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

t_token	*is_here(t_cmd *cmd)
{
	t_cmd	*tmp;
	t_token	*token;
	t_token	*ret;

	tmp = cmd;
	while (tmp)
	{
		token = cmd->redir;
		while (token)
		{
			if (token->type == rdin)
			{
				ret = token;
			}	
			else if (token->type == rin)
				ret = NULL;
			token = token->next;
		}
		tmp = tmp->next;
	}
	return (ret);
}
