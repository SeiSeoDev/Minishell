/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 21:37:31 by user42            #+#    #+#             */
/*   Updated: 2022/03/01 18:12:53 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (!link)
		return (NULL);
	i = 0;
	j = 0;
	if (res)
	{
		while (res && link && res[i])
			link[j++] = res[i++];
		free(res);
	}
	if (str)
	{
		i = 0;
		while (str && link && str[i])
			link[j++] = str[i++];
		free(str);
	}
	link[j++] = '\n';
	link[j] = '\0';
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
	return (link_here(res, s));
}

static char	*heredoc_extra(t_token *redir, char *res, int ex)
{
	char	*str;

	str = NULL;
	if (res)
	{
		free(res);
		res = NULL;
	}
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
	return (res);
}

char	*heredoc(t_cmd *cmd)
{
	char	*res;
	t_token	*redir;

	if (!cmd || !cmd->redir)
		return (NULL);
	res = NULL;
	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == lim)
			res = heredoc_extra(redir, res, 0);
		redir = redir->next;
	}
	if (!res)
		res = ft_strdup("");
	return (res);
}
