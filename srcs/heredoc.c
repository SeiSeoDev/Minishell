/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 21:37:31 by user42            #+#    #+#             */
/*   Updated: 2022/03/01 08:50:55 by dasanter         ###   ########.fr       */
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

char	*heredoc(t_token *redir)
{
	char	*str;
	char	*res;
	int		ex;

	ex = 0;
	res = NULL;
	if (!redir || !redir->str)
		return (NULL);
	redir->str = del_unused_quot(redir->str);
	while (ex == 0)
	{
		str = readline("\e[1m\e[31m\002"">""\001\e[0m\002");
		if (str && ft_strcmp(redir->str, str) != 0)
			res = read_here(str, res);
		else
			ex = 1;
	}
	if (!res)
		res = ft_strdup("");
	if (str)
		free(str);
	return (res);
}
