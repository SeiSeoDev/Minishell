/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 16:31:03 by tamigore          #+#    #+#             */
/*   Updated: 2022/01/28 17:14:16 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	free(res);
	return (link);
}

static char	*heredoc(t_token *redir)
{
	char	*str;
	char	*res;
	int		quot;

	quot = 0;
	res = NULL;
	if (ft_strchr(redir->str, '"') || ft_strchr(redir->str, '\''))
		quot = 1;
	redir->str = del_unused_quot(redir->str);
	str = readline("\e[1m\e[31m\002"">""\001\e[0m\002");
	while (ft_strcmp(str, redir->str))
	{
		if (quot)
			res = link_here(res, expend_words(NULL, str));
		else
			res = link_here(res, str);
		str = readline("\e[1m\e[31m\002"">""\001\e[0m\002");
	}
	if (!res)
		res = ft_strdup("");
	return (res);
}

void	close_fd(t_cmd *cmd)
{
	t_token	*token;

	token = cmd->redir;
	while (token)
	{
		if (token->fd != 1 && token->fd != 0)
		{
			printf("close fd -> %d", token->fd);
			close(token->fd);
		}
		token = token->next;
	}
}

char	*fill_fd(t_cmd *cmd)
{
	char	*doc;
	t_token *token;

	token = cmd->redir;
	while (token)
	{
		if (token->type == rout)
		{
			cmd->fdout = open(token->next->str, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			token->fd = cmd->fdout;
		}
		else if (token->type == rdout)
		{	
			cmd->fdout = open(token->next->str, O_CREAT | O_WRONLY | O_APPEND, 0644);
			token->fd = cmd->fdout;
		}
		else if (token->type == rin)
		{
			cmd->fdin = open(token->next->str,  O_RDONLY);
			if (cmd->fdin)
			token->fd = cmd->fdin;
		}
		else if (token->type == rdin)
		{
			doc = heredoc(token->next);
			if (!doc)
				exfree(cmd, "heredoc failure\n", 'c', 1);
			return (doc);
		}
		token = token->next;
	}
	return (NULL);
}

int	find_file(char *path)
{
	struct stat	sb;
/*
**	int			res;
*/
	if (!path)
		return (0);
	if (lstat(path, &sb) == -1)
		return (0);
/*	printf("Type de fichier : ");
**	res = (sb.st_mode & S_IFMT);
**	if (res == S_IFBLK)
**		printf("périphérique de bloc\n");
**	else if (res == S_IFCHR)
**		printf("périphérique de caractère\n");
**	else if (res == S_IFDIR)
**		printf("répertoire\n");
**	else if (res == S_IFIFO)
**		printf("FIFO/tube\n");
**	else if (res == S_IFLNK)
**		printf("lien symbolique\n");
**	else if (res == S_IFREG)
**		printf("fichier ordinaire\n");
**	else if (res == S_IFSOCK)
**		printf("socket\n");
**	else
**		printf("inconnu ?\n");
*/
	return (1);
}