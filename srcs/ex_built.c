/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_built.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 16:07:14 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/28 16:49:56 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ex_echo(t_cmd *cmd)
{
	int		n;
	t_token	*arg;

	arg = cmd->arg->next;
	n = 0;
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

int	ex_cd(t_cmd *cmd)
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
		return (0); // rajouter le msg d erreur
	else
	{
		handler(3, NULL, "OLDPWD", handler(3, NULL, "PWD", NULL)->val);
		handler(3, NULL, "PWD", getcwd(buf, 4096));
	}
	return (1);
}

int	ex_pwd(t_cmd *cmd)
{
	t_env	*myenv;

	(void)cmd;
	myenv = handler(3, NULL, "PWD", NULL);
	write(1, myenv->val, ft_strlen(myenv->val));
	write(1, "\n", 1);
	return (0);
}