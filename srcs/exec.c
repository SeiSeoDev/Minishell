/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 15:50:00 by dasanter          #+#    #+#             */
/*   Updated: 2022/02/27 14:13:23 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**creat_arg(t_cmd *cmd)
{
	t_token	*tmp;
	char	**arg;
	int		i;

	if (!cmd)
		return (NULL);
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

	if (!env || !env->val)
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

static void	exe_cmd(t_cmd *cmd)
{
	t_env	*env;
	char	**arg;
	char	*exe;
	char	**all;

	env = handler(3, NULL, "PATH", NULL);
	exe = creat_exe(env, cmd);
	if (!exe || !ft_strncmp(cmd->arg->str, "/", 1))
	{
		if (!ft_strcmp(cmd->arg->str, "/"))
		{
			fprintf(stderr, "Minishell: %s: Is a directory\n", cmd->arg->str);
			exfree(cmd, NULL, 'c', 126);
		}
		env = handler(3, NULL, "PWD", NULL);
		if (env)
			exe = ft_strjoin(env->val, &cmd->arg->str[1]);
		if (find_file(exe))
		{
			if (!ft_strncmp(cmd->arg->str, "/", 1))
			{
				fprintf(stderr, "Minishell: %s: Is a directory\n", cmd->arg->str);
				exfree(cmd, NULL, 'c', 126);
			}
		}
		else
		{
			if (!ft_strncmp(cmd->arg->str, "/", 1))
			{
				fprintf(stderr, "Minishell: %s: No such file or directory\n", cmd->arg->str);
				exfree(cmd, NULL, 'c', 127);
			}
		}
	}
	env = handler(3, NULL, NULL, NULL);
	all = get_env(env);
	arg = creat_arg(cmd);
	execve(exe, arg, all);
	fprintf(stderr, "Minishell: %s: command not found\n", cmd->arg->str);
	if (arg)
		free(arg);
	if (exe)
		free(exe);
	if (all)
		ft_free_tab(all);
	exfree(cmd, NULL, 'c', 127);
}

void	exec(t_cmd *cmd)
{
	int		fdok;

	if (cmd && cmd->redir)
	  	fill_fd(cmd);
	dup2(cmd->fdin, STDIN_FILENO);
	fdok = isntopen(cmd);
	if (cmd != NULL && cmd->arg != NULL && !fdok && cmd->arg->str)
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
			exe_cmd(cmd);
		}
	}
	close_fd(cmd);
}
