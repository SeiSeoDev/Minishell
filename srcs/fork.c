/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 17:00:04 by tamigore          #+#    #+#             */
/*   Updated: 2022/03/01 18:53:15 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_state = 0;

void	sig_handler(int sig)
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

static void	wait_process(t_cmd *cmd)
{
	int		status;
	t_cmd	*tmp;

	status = 0;
	tmp = cmd;
	while (tmp)
	{
		if (!is_built(tmp))
		{
			if (tmp->pid && waitpid(tmp->pid, &status, 0) == -1)
				write(STDERR_FILENO, "ERROR\n", 6);
			if (WIFEXITED(status))
				tmp->exit = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				tmp->exit = WTERMSIG(status) + 128;
		}
		if (!tmp->next)
			handler(tmp->exit, NULL, "?", NULL);
		tmp = tmp->next;
	}
}

void	child_extra(t_cmd *cmd, t_cmd *tmp, int *pipefd, char *doc)
{
	signal(SIGQUIT, SIG_DFL);
	if (tmp->next)
		dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	exec(tmp, doc);
	exfree(cmd, NULL, 'c', 1);
}

void	child(t_cmd *cmd, t_cmd *tmp, int *pipefd, int i)
{
	int		fd_in;
	char	*doc;

	doc = NULL;
	fd_in = dup(STDIN_FILENO);
	while (tmp && pipefd)
	{
		if (is_herdoc(tmp))
			doc = heredoc(tmp);
		pipe(&pipefd[i * 2]);
		tmp->pid = fork();
		if (tmp->pid == 0)
		{
			if (get_nbpipe(cmd) != get_nbpipe(tmp))
				dup2(fd_in, STDIN_FILENO);
			child_extra(cmd, tmp, &pipefd[i * 2], doc);
		}
		if (doc)
			ft_memdel((void **)&doc);
		dup2(pipefd[i * 2], fd_in);
		close(pipefd[i * 2]);
		close(pipefd[(i++) * 2 + 1]);
		tmp = tmp->next;
	}
	close(fd_in);
}

void	parent(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		*pipefd;
	char	*doc;

	doc = NULL;
	g_state = 1;
	tmp = cmd;
	handler(0, NULL, "?", NULL);
	pipefd = malloc((get_nbpipe(cmd) * 2) * sizeof(int));
	if (get_nbpipe(cmd) == 1 && is_built(cmd) && is_herdoc(cmd))
		doc = heredoc(cmd);
	if (get_nbpipe(cmd) == 1 && is_built(cmd))
		exec(cmd, doc);
	else
	{
		child(cmd, tmp, pipefd, 0);
		wait_process(cmd);
	}
	g_state = 0;
	if (pipefd)
		free(pipefd);
	if (cmd)
		free_cmd(cmd);
}
