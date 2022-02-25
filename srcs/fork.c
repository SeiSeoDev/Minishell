/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 17:00:04 by tamigore          #+#    #+#             */
/*   Updated: 2022/02/25 17:15:27 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	gl_state = 0;

static int	get_nbpipe(t_cmd *cmd)
{
	int i;
	t_cmd *tmp;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
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
void sig_handler2(int sig)
{
	if (sig == SIGINT && gl_state == 0)
	{
		ft_putchar_fd('\n', 1);
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (sig == SIGINT && gl_state == 1)
	{
		ft_putchar_fd('\n', 1);
		rl_redisplay();
	}
	else if (sig == SIGQUIT && gl_state == 1)
	{
		ft_putstr_fd("Quit (core dumped)\n", 2);
	}
	else if (sig == SIGQUIT)
		ft_putstr_fd("\b\b  \b\b", 1);
}

void sig_heredoc(int sig)
{
	rl_on_new_line();
	(void)sig;
	ft_putchar_fd('\n', 1);
	exit(0);
}

int	wait_process(t_cmd *cmd) 
{ 
	int		status;
	t_cmd	*tmp;

	status = 0;
	tmp = cmd;
	while (tmp)
	{
		if (!is_built(tmp))
		{
			 if (waitpid(tmp->pid, &status, 0) == -1)
				write(STDERR_FILENO, "ERROR\n", 6);
			 if (WIFEXITED(status))
				tmp->exit = WEXITSTATUS(status);
			 else if (WIFSIGNALED(status))
				tmp->exit = WTERMSIG(status) + 128;
		}
		tmp = tmp->next;
	}
	tmp = cmd;
	while (tmp && tmp->next)
	{
		if (tmp->exit == 131)
			write(1, "^Quit (core dumped)\n", 21);
		tmp = tmp->next;
	}
	if (tmp->exit == 131)
		write(1, "^Quit (core dumped)\n", 21);
	return (tmp->exit);
}

int	herdoc(t_cmd *cmd)
{
	t_token *redir;
	int		res;

	res = 0;
	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == rdin)
			res = 1;
		else if (redir->type == rin)
			res = 0;
		redir = redir->next;
	}
	return (res);
}

void	child(t_cmd *cmd)
{
	int		*pipefd;
	int		i;
	int		fd_in;
	t_cmd	*tmp;
	
	tmp = cmd;
	if (!tmp)
		return ;
	pipefd = malloc((get_nbpipe(cmd) * 2) * sizeof(int));
	fd_in = dup(STDIN_FILENO);
	gl_state = 1;
	i = 0;
	if (get_nbpipe(cmd) == 1 && is_built(cmd))
		exec(cmd);
	else
	{
		while (tmp)
		{
			pipe(&pipefd[i * 2]);
			tmp->pid = fork();
			if (tmp->pid == 0)
			{
				signal(SIGQUIT, SIG_DFL);
				if (get_nbpipe(cmd) != get_nbpipe(tmp) || herdoc(tmp))
					dup2(fd_in, STDIN_FILENO);
				if (tmp->next)
					dup2(pipefd[i * 2 + 1], STDOUT_FILENO);
				close(pipefd[i * 2]);
				close(pipefd[i * 2 + 1]);
				close(fd_in);
				exec(tmp);
				exfree(tmp, NULL, 'c', 1);
			}
			dup2(pipefd[i * 2], fd_in);
			close(pipefd[i * 2]);
			close(pipefd[i * 2 + 1]);
			i++;
			tmp = tmp->next;
		}
	}
	close(fd_in);
	i = wait_process(cmd);
	handler(i, NULL, "?", NULL);
	gl_state = 0;
	if (cmd)
		free_cmd(cmd);
	if (pipefd)
		free(pipefd);
}