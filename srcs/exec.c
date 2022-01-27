/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 15:50:00 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/27 17:01:53 by tamigore         ###   ########.fr       */
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

static char *creat_exe(t_env *env, t_cmd *cmd)
{
	char	*path;
	char	*exe;
	int		i;
	int		j;
	
	if (!env->val)
		return (NULL);
	i = 0;
	j = i;
	while (env->val[i])
	{
		if (env->val[i] == ':')
		{
			path = ft_strndup(&env->val[j], i - j);
			if (!path)
				return (NULL);
			path = ft_free_join(path, "/", 1);
			if (!path)
				return (NULL);
			exe = ft_strjoin(path, cmd->arg->str);
			free(path);
			if (find_file(exe))
				return (exe);
			free(exe);
			j = i + 1;
		}
		i++;
	}
	return (NULL);
}

static int	exe_prog(t_cmd *cmd)
{
	t_env	*env;
	char	**arg;
	char	*exe;
	char	**all;

	env = handler(3, NULL, NULL, NULL);
	if (!env)
		return (0);
	all = get_env(env);
	if (!all)
		return (0);
	env = handler(3, NULL, "PWD", NULL);
	exe = ft_strjoin(env->val, &cmd->arg->str[1]);
	arg = creat_arg(cmd);
	if (!env || !exe || !arg || !(find_file(exe)))
	{
		if (exe)
			free(exe);
		if (arg)
			free(arg);
		free_tab(all);
		return (0);
	}
	if (!ft_strcmp(cmd->arg->str, "./minishell"))
		handler(3, NULL, "SHLVL", ft_itoa(ft_atoi(handler(3, NULL, "SHLVL", NULL)->val) + 1));
	if 	(execve(exe, arg, all) == -1)
		printf("command failed\n");
	free(arg);
	free(exe);
	free_tab(all);
	return (1);
}

static int	exe_cmd(t_cmd *cmd)
{
	t_env	*env;
	char	**arg;
	char	*exe;
	char	**all;

	env = handler(3, NULL, "PATH", NULL);
	if (!env)
		return (0);
	exe = creat_exe(env, cmd);
	if (!exe)
	{
		printf("deadly ?\n");
		return (0);
	}
	arg = creat_arg(cmd);
	env = handler(3, NULL, NULL, NULL);
	if (!env)
		return (0);
	all = get_env(env);
	if (!arg || !all)
	{
		free(exe);
		if (all)
			free_tab(all);
		if (arg)
			free(arg);
		return (0);
	}
	if 	(execve(exe, arg, all) == -1)
	{
		printf("command failed\n");
	}
	if (arg)
		free(arg);
	if (exe)
		free(exe);
	if (all)
		free(all);
	printf("end\n");
	return (1);
}

char    **exec(t_cmd *cmd)
{
	if (cmd->redir)
		fill_fd(cmd);
	dup2(cmd->fdin, STDIN_FILENO);
	if (cmd != NULL && cmd->arg != NULL)
	{

		if (!ft_strcmp(cmd->arg->str, "echo"))
			ex_echo(cmd);
		else if (!ft_strcmp(cmd->arg->str, "cd"))
		{
			ex_cd(cmd);
		}
		else if (!ft_strcmp(cmd->arg->str, "pwd"))
			ex_pwd(cmd);
		else if (!ft_strcmp(cmd->arg->str, "env"))
			ex_env(cmd);
		else if (!ft_strcmp(cmd->arg->str, "unset"))
			ex_unset(cmd);
		else if (!ft_strcmp(cmd->arg->str, "export"))
			ex_port(cmd);
		else
		{
			if (!ft_strncmp(cmd->arg->str, "./", 2))
				exe_prog(cmd);
			else if (!exe_cmd(cmd))
				printf("Minishell: %s: command not found\n", cmd->arg->str);
		}
	}
	printf("fdout : %d\n", cmd->fdout);
	close_fd(cmd);
	return (get_env(handler(3, NULL, NULL, NULL)));
}
