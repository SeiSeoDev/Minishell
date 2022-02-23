/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 15:50:00 by dasanter          #+#    #+#             */
/*   Updated: 2022/02/23 14:09:07 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**creat_arg(t_cmd *cmd)
{
	t_token	*tmp;
	char	**arg;
	int		i;

	tmp = cmd->arg;
	i = 0;
	while (tmp)
	{
		tmp = tmp->next;
		i++;
	}
	arg = malloc(sizeof(char *) * (i + 1));
	if (!arg)
		return (NULL);
	tmp = cmd->arg;
	i = 0;
	while (tmp)
	{
		arg[i++] = tmp->str;
		tmp = tmp->next;
	}
	arg[i] = NULL;
	return (arg);
}

static char	*creat_exe(t_env *env, t_cmd *cmd)
{
	char	*exe;
	int		i;
	int		j;

	if (!env->val)
		return (NULL);
	i = -1;
	j = i + 1;
	while (env->val[++i])
	{
		if (env->val[i] == ':')
		{
			exe = ft_strndup(&env->val[j], i - j);
			if (!exe)
				return (NULL);
			exe = ft_free_join(exe, "/", 1);
			if (!exe)
				return (NULL);
			exe = ft_free_join(exe, cmd->arg->str, 1);
			if (find_file(exe))
				return (exe);
			free(exe);
			j = i + 1;
		}
	}
	return (NULL);
}

static int	exe_cmd(t_cmd *cmd)
{
	t_env	*env;
	char	**arg;
	char	*exe;
	char	**all;
	int		res;

	env = handler(3, NULL, "PATH", NULL);
	exe = creat_exe(env, cmd);
	if (!exe)
	{
		if (!ft_strncmp(cmd->arg->str, ".", 1))
		{
			env = handler(3, NULL, "PWD", NULL);
			if (env)
				exe = ft_strjoin(env->val, &cmd->arg->str[1]);
		}
		else if (!ft_strncmp(cmd->arg->str, "/", 1))
		{
			if (find_file(cmd->arg->str) || !ft_strcmp(cmd->arg->str, "/"))
			{
				printf("Minishell: %s: Is a directory\n", cmd->arg->str);
				return (-1);
			}
			else
				return (0);
		}
		else
			return (0);
	}
	env = handler(3, NULL, NULL, NULL);
	all = get_env(env);
	arg = creat_arg(cmd);
	res = find_file(exe);
	if (!arg || !all || !exe || !res)
		res = 0;
	else if (access(exe, X_OK) == -1)
	{
		printf("Minishel: %s: Permission denied\n", cmd->arg->str);
		res = -1;
	}
	else if (execve(exe, arg, all) == -1)
		res = 0;
	else
		res = 1;
	if (arg)
		free(arg);
	if (exe)
		free(exe);
	if (all)
		ft_free_tab(all);
	return (res);
}

void	exec(t_cmd *cmd)
{
	int		fdok;

	if (cmd->redir)
	  	fill_fd(cmd);
	dup2(cmd->fdin, STDIN_FILENO);
	fdok = isntopen(cmd);
	if (cmd != NULL && cmd->arg != NULL && !fdok)
	{
		if (!ft_strcmp(cmd->arg->str, "echo"))
			ex_echo(cmd);
		else if (!ft_strcmp(cmd->arg->str, "cd"))
			ex_cd(cmd);
		else if (!ft_strcmp(cmd->arg->str, "pwd"))
			ex_pwd(cmd);
		else if (!ft_strcmp(cmd->arg->str, "env"))
			ex_env(cmd);
		else if (!ft_strcmp(cmd->arg->str, "unset"))
			ex_unset(cmd);
		else if (!ft_strcmp(cmd->arg->str, "export"))
			ex_port(cmd);
		else if (!ft_strcmp(cmd->arg->str, "exit"))
			exfree(cmd, "exit", 'c', 0);
		else if (!fdok)
		{
			dup2(cmd->fdout, STDOUT_FILENO);
			if (exe_cmd(cmd) == 0)
			{
				if (!ft_strncmp(cmd->arg->str, "/", 1))
					printf("Minishell: %s: No such file or directory\n", cmd->arg->str);
				else
					printf("Minishell: %s: command not found\n", cmd->arg->str);
			}
		}
	}
	close_fd(cmd);
}
