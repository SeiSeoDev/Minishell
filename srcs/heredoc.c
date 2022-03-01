/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 21:37:31 by user42            #+#    #+#             */
/*   Updated: 2022/03/01 15:40:37 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_heredoc(int sig)
{
	rl_on_new_line();
	(void)sig;
	ft_putchar_fd('\n', 1);
	exit(0);
}

int	is_herdoc(t_cmd *cmd)
{
	t_token	*redir;

	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == rdin)
			return (1);
		redir = redir->next;
	}
	return (0);
}

static char	*link_here(char *res, char *str)
{
	int		i;
	int		j;
	char	*link;

	link = malloc(sizeof(char) * (ft_strlen(res) + ft_strlen(str) + 2));
	i = 0;
	j = 0;
	if (res)
	{
		while (res[i])
			link[j++] = res[i++];
	}
	if (str)
	{
		i = 0;
		while (str[i])
			link[j++] = str[i++];
	}
	link[j++] = '\n';
	link[j] = '\0';
	if (res)
		free(res);
	return (link);
}

static char	*read_here(char *s, char *res)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '$' && quot_status(s, i) == 0 && (ft_isalnum(s[i + 1])
				|| s[i + 1] == '_' || s[i + 1] == '?' || s[i + 1] == '$'))
			s = expend_words(s, i);
		i++;
	}
	res = link_here(res, s);
	if (!res)
		return (NULL);
	free(s);
	return (res);
}

char	*heredoc(t_cmd *cmd)
{
	char	*str;
	char	*res;
	int		ex;
	t_token	*redir;

	ex = 0;
	res = NULL;
	if (!cmd->redir || !cmd->redir->str)
		return (NULL);
	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == lim)
		{
			redir->str = del_unused_quot(redir->str);
			while (ex == 0)
			{
				str = readline("\e[1m\e[31m\002"">""\001\e[0m\002");
				if (str && ft_strcmp(redir->str, str) != 0)
					res = read_here(str, res);
				else
					ex = 1;
			}
			if (str)
				free(str);
		}
		redir = redir->next;
	}
	if (!res)
		res = ft_strdup("");
	return (res);
}
