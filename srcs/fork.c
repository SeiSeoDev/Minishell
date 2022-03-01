/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 17:00:04 by tamigore          #+#    #+#             */
/*   Updated: 2022/03/01 15:47:01 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_state = 0;

static int	get_nbpipe(t_cmd *cmd)
{
	int		i;
	t_cmd	*tmp;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

void	sig_handler2(int sig)
{
	if (sig == SIGINT && g_state == 0)
	{
		ft_putchar_fd('\n', 1);
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
		handler(130, NULL, "?", NULL);
	}
	else if (sig == SIGINT && g_state == 1)
	{
		ft_putchar_fd('\n', 1);
		rl_redisplay();
		handler(130, NULL, "?", NULL);
	}
	else if (sig == SIGQUIT && g_state == 1)
	{
		ft_putstr_fd("Quit (core dumped)\n", 2);
	}
	else if (sig == SIGQUIT)
		ft_putstr_fd("\b\b  \b\b", 1);
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
	while (tmp && tmp->exit == 131)
		tmp = tmp->next;
	if (tmp && tmp->exit == 131)
		write(1, "^Quit (core dumped)\n", 21);
	return (tmp->exit);
}

void	child(t_cmd *cmd, t_cmd *tmp, int *pipefd, int *i)
{
	int		fd_in;
	char	*doc;

	doc = NULL;
	fd_in = dup(STDIN_FILENO);
	while (tmp && pipefd)
	{
		if (is_herdoc(tmp))
			doc = heredoc(tmp);
		pipe(&pipefd[*i * 2]);
		tmp->pid = fork();
		if (tmp->pid == 0)
		{
			signal(SIGQUIT, SIG_DFL);
			if (get_nbpipe(cmd) != get_nbpipe(tmp))
				dup2(fd_in, STDIN_FILENO);
			if (tmp->next)
				dup2(pipefd[*i * 2 + 1], STDOUT_FILENO);
			close(pipefd[*i * 2]);
			close(pipefd[*i * 2 + 1]);
			exec(tmp, doc);
			exfree(cmd, NULL, 'c', 1);
		}
		dup2(pipefd[*i * 2], fd_in);
		close(pipefd[*i * 2]);
		close(pipefd[((*i)++) * 2 + 1]);
		tmp = tmp->next;
	}
	close(fd_in);
}

void	parent(t_cmd *cmd)
{
	int		i;
	t_cmd	*tmp;
	int		*pipefd;
	char	*doc;

	doc = NULL;
	if (!cmd)
		return ;
	g_state = 1;
	tmp = cmd;
	i = 0;
	pipefd = malloc((get_nbpipe(cmd) * 2) * sizeof(int));
	if (is_built(cmd) && is_herdoc(cmd))
		doc = heredoc(cmd);
	if (get_nbpipe(cmd) == 1 && is_built(cmd))
		exec(cmd, doc);
	else
	{
		child(cmd, tmp, pipefd, &i);
		i = wait_process(cmd);
		handler(i, NULL, "?", NULL);
	}
	g_state = 0;
	if (pipefd)
		free(pipefd);
	if (cmd)
		free_cmd(cmd);
}
