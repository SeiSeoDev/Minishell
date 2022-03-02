/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 15:50:00 by dasanter          #+#    #+#             */
/*   Updated: 2022/03/02 17:25:04 by tamigore         ###   ########.fr       */
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

static char	*creat_exe(t_env *env, t_cmd *cmd, int j, int i)
{
	char	*exe;

	if (!env || !env->val)
		return (NULL);
	while (env->val[i])
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
		i++;
	}
	return (NULL);
}

static void	exe_dir(t_cmd *c, char *s)
{
	if (ft_search(c->arg->str, '/'))
	{
		if (chdir(c->arg->str) == -1)
		{
			if (!find_file(c->arg->str))
			{
				print_err(s, ": No such file or directory\n");
				exfree(c, NULL, 'c', 127);
			}
		}
		else
		{
			print_err(s, ": Is a directory\n");
			exfree(c, NULL, 'c', 126);
		}
	}
}

static char	*exe_extra(t_cmd *c, t_env *env, char *exe, char *s)
{
	exe_dir(c, s);
	if (c->arg->str[0] == '.')
	{
		env = handler(3, NULL, "PWD", NULL);
		if (env)
		{
			exe = ft_strjoin(env->val, "/");
			exe = ft_free_join(exe, c->arg->str, 1);
			if (access(exe, X_OK) == -1)
			{
				print_err(c->arg->str, ": Permission denied\n");
				if (exe)
					free(exe);
				exfree(c, NULL, 'c', 126);
			}
		}
	}
	else
		exe = creat_exe(env, c, 0, 0);
	if (!exe)
		exe = ft_strdup(c->arg->str);
	return (exe);
}

void	exe_cmd(t_cmd *cmd)
{
	t_env	*env;
	char	**arg;
	char	*exe;
	char	**all;

	isntopen(cmd);
	env = handler(3, NULL, "PATH", NULL);
	exe = exe_extra(cmd, env, NULL, cmd->arg->str);
	env = handler(3, NULL, NULL, NULL);
	all = get_env(env);
	arg = creat_arg(cmd);
	execve(exe, arg, all);
	print_err(cmd->arg->str, ": command not found\n");
	if (arg)
		free(arg);
	if (exe)
		free(exe);
	if (all)
		ft_free_tab(all);
	exfree(cmd, NULL, 'c', 127);
}
