/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 15:50:00 by dasanter          #+#    #+#             */
/*   Updated: 2022/02/25 18:56:36 by dasanter         ###   ########.fr       */
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

static int	exe_cmd(t_cmd *cmd)
{
	t_env	*env;
	char	**arg;
	char	*exe;
	char	**all;
	int		res;

	env = handler(3, NULL, "PATH", NULL);
	exe = NULL;
	if (ft_strncmp(cmd->arg->str, ".", 1) || ft_strncmp(cmd->arg->str, "/", 1))
		exe = creat_exe(env, cmd);
	else
	{
		if (!ft_strcmp(cmd->arg->str, "/"))
		{
			printf("Minishell: %s: Is a directory\n", cmd->arg->str);
			exfree(cmd, NULL, 'c', 126);
		}
		env = handler(3, NULL, "PWD", NULL);
		if (env)
			exe = ft_strjoin(env->val, &cmd->arg->str[1]);
		if (find_file(exe))
		{
			if (!ft_strncmp(cmd->arg->str, "/", 1))
			{
				printf("Minishell: %s: Is a directory\n", cmd->arg->str);
				exfree(cmd, NULL, 'c', 126);
			}
			printf("Minishell: %s: command not found\n", cmd->arg->str);
			exfree(cmd, NULL, 'c', 127);
		}
		else
		{
			if (!ft_strncmp(cmd->arg->str, "/", 1))
				printf("Minishell: %s: No such file or directory\n", cmd->arg->str);
			else
				printf("Minishell: %s: command not found\n", cmd->arg->str);
			exfree(cmd, NULL, 'c', 127);
		}
	}
	if (exe)
		printf("exe = %s\n", exe);
	else
	{
		printf("Minishell: %s: command not found\n", cmd->arg->str);
		exfree(cmd, NULL, 'c', 127);
	}
	if (cmd)
		print_cmd(cmd);
	else
	{
		printf("Minishell: %s: command not found\n", cmd->arg->str);
		exfree(cmd, NULL, 'c', 127);
	}
	env = handler(3, NULL, NULL, NULL);
	all = get_env(env);
	arg = creat_arg(cmd);
	res = find_file(exe);
	if (!arg || !all || !exe || !res)
		res = -1;
	else if (access(exe, X_OK) == -1)
	{
		printf("Minishel: %s: Permission denied\n", cmd->arg->str);
		ctrfree(cmd, NULL, 'c', 1);
		return (-1);
	}
	else if (execve(exe, arg, all) == -1)
	{
		if (errno == ENOTDIR)// A component of the path prefix is not a directory.
			res = errno;
		if (errno == ENAMETOOLONG)// A component of a pathname exceeded 255 characters, or an entire path name exceeded 1023 characters.
			res = errno;
		if (errno == ENOENT)// The new process file does not exist.
			res = errno;
		if (errno == ELOOP)// Too many symbolic links were encountered in translating the pathname.
			res = errno;
		if (errno == EACCES)// Search permission is denied for a component of the path prefix.
			res = errno;	// The new process file is not an ordinary file. The new process file mode denies execute permission.
		if (errno == ENOEXEC)// The new process file has the appropriate access permission, but has an invalid magic number in its header.
			res = errno;
		if (errno == ENOMEM)// The new process requires more virtual memory than is allowed by the imposed maximum (getrlimit(2POSIX)).
			res = errno;
		if (errno == E2BIG)// The number of bytes in the new process argument list is larger than the system-imposed limit.
			res = errno;
		if (errno == EFAULT)// The new process file is not as long as indicated by the size values in its header. path, argv, or envp point to an illegal address.
			res = errno;
		if (errno == EIO)// An I/O error occurred while reading from the file system.
			res = errno;
		if (errno == EINVAL)// A system error occurred.
			res = errno;
		if (errno == EPERM)// The operation is performed in system address space. The operation is performed by a multi-threaded process.
			res = errno;
		printf("Error in execve\n");
	}
	if (arg)
		free(arg);
	if (exe)
		free(exe);
	if (all)
		ft_free_tab(all);
	return (res);
}

t_token *is_here(t_cmd *cmd)
{
	t_cmd *tmp;
	t_token *token;
	t_token *ret;
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

void	exec(t_cmd *cmd)
{
	int		fdok;
	int		res;
	int		pitab[2];
	int		pid;
	char 	*herestr;
	pid = cmd->pid;
	herestr = NULL;
	if (cmd && cmd->redir)
	  	herestr = fill_fd(cmd);
	fdok = isntopen(cmd);
	if (is_here(cmd))
	{
		pipe(pitab);
		printf("pipe : %d --> %d\n", pitab[0], pitab[1]);
		printf("HEREDOCSTR : %s\n", herestr);
		dup2(pitab[0], STDIN_FILENO);
		dup2(pitab[1], STDOUT_FILENO);
		printf("pipe : %d --> %d\n", pitab[0], pitab[1]);
		write(pitab[1], herestr, ft_strlen(herestr));
	}
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
			res = exe_cmd(cmd);
			// if (res == 0)
			// {
			// 	if (!ft_strncmp(cmd->arg->str, "/", 1))
			// 		printf("Minishell: %s: No such file or directory\n", cmd->arg->str);
			// 	else
			// 		printf("Minishell: %s: command not found\n", cmd->arg->str);
			// 	return (127);
			// }
		}
	}
	close_fd(cmd);
}
