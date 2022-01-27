/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 17:00:04 by tamigore          #+#    #+#             */
/*   Updated: 2022/01/27 17:01:00 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	is_built(t_cmd *cmd)
{
	if (!ft_strcmp(cmd->arg->str, "echo"))
		return (1);
	else if (!ft_strcmp(cmd->arg->str, "cd"))
		return (1);
	else if (!ft_strcmp(cmd->arg->str, "pwd"))
		return (1);
	else if (!ft_strcmp(cmd->arg->str, "env"))
		return (1);
	else if (!ft_strcmp(cmd->arg->str, "unset"))
		return (1);
	else if (!ft_strcmp(cmd->arg->str, "export"))
		return (1);
	return (0);
}

void	child(t_cmd *cmd)
{
	int		*pipefd;
	int		*pitab;
	int		i;
	int		fd_in;
	int		status;
	t_cmd	*tmp;
	
	tmp = cmd;
	pipefd = malloc((get_nbpipe(cmd) * 2) * sizeof(int));
	pitab = malloc((get_nbpipe(cmd)) * sizeof(int));
	fd_in = dup(STDIN_FILENO);
	i = 0;
	if (get_nbpipe(cmd) == 1 && is_built(cmd))
	{
		exec(cmd);
		return;
	}
	while (i < get_nbpipe(cmd))
	{
		pipe(&pipefd[i * 2]);
		pitab[i] = fork();
		if (pitab[i] == 0)
		{
			if (i != 0)
				dup2(fd_in, STDIN_FILENO);
			if ((i + 1) != get_nbpipe(cmd))
				dup2(pipefd[i * 2 + 1], STDOUT_FILENO);
			close(pipefd[i * 2]);
			close(pipefd[i * 2 + 1]);
			close(fd_in);
			exec(tmp);
			exit_free(cmd, NULL, 'c');

		}
		dup2(pipefd[i * 2], fd_in);
		close(pipefd[i * 2]);
		close(pipefd[i * 2 + 1]);
		i++;
		tmp = tmp->next;
	}
	close(fd_in);
    i = -1;
    while (++i < get_nbpipe(cmd))
		waitpid(pitab[i], &status, 0);
	free_cmd(cmd);
	free(pipefd);
	free(pitab);
}