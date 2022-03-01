/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 16:31:03 by tamigore          #+#    #+#             */
/*   Updated: 2022/02/25 18:18:20 by dasanter         ###   ########.fr       */
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
	if (res)
		free(res);
	return (link);
}

char	*heredoc(t_token *redir)
{
	char	*str;
	char	*res;
	int		quot;
	int		ex;

	quot = 0;
	ex = 0;
	res = NULL;
	if (!redir || !redir->str)
		return (NULL);
	if (ft_strchr(redir->str, '"') || ft_strchr(redir->str, '\''))
		quot = 1;
	redir->str = del_unused_quot(redir->str);
	while (ex == 0)
	{
		str = readline("\e[1m\e[31m\002"">""\001\e[0m\002");
		if (str && ft_strcmp(redir->str, str) != 0)
		{
			if (quot == 0)
				str = expend_words(str);
			res = link_here(res, str);
			if (!res)
				return (NULL);
			free(str);
		}
		else
			ex = 1;
	}
	if (!res)
		res = ft_strdup("");
	if (str)
		free(str);
	return (res);
}

void	close_fd(t_cmd *cmd)
{
	t_token	*token;

	token = cmd->redir;
	while (token)
	{
		if (token->fd != 1 && token->fd != 0)
			close(token->fd);
		token = token->next;
	}
}

int isntopen(t_cmd *cmd)
{
	t_token *token;

	token = cmd->redir;
	if (!token)
		return (0);
	if (token->type == rout || token->type == rdout)
	{
		if (cmd->fdout <= 0)
		{
			if (access(token->next->str, F_OK ) != 0 )
				printf("Minishell: %s: No such file or directory\n", token->next->str);
			else
				printf("Minishell: %s: Permission denied\n", token->next->str);
			return (1);
		}
	}
	else if (token->type == rin)
	{
		if (cmd->fdin <= 0)
		{
			if (access(token->next->str, F_OK ) != 0)
				printf("Minishell: %s: No such file or directory\n", token->next->str);
			else
				printf("Minishell: %s: Permission denied\n", token->next->str);
			return (1);
		}
	}
	return (0);
}

char	*fill_fd(t_cmd *cmd)
{
	char	*doc;
	t_token *token;

	if (!cmd)
		return (NULL);
	token = cmd->redir;
	doc = NULL;
	while (token)
	{
		if (token->type == rout)
		{
			cmd->fdout = open(token->next->str, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (cmd->fdin < 0)
			{
				printf("Minishell: %s: Permission denied\n", token->next->str);
				exfree(cmd, NULL, 'c', 1);
			}
			token->fd = cmd->fdout;
		}
		else if (token->type == rdout)
		{
			cmd->fdout = open(token->next->str, O_CREAT | O_WRONLY | O_APPEND, 0644);
			if (cmd->fdin < 0)
			{
				printf("Minishell: %s: Permission denied\n", token->next->str);
				exfree(cmd, NULL, 'c', 1);
			}
			token->fd = cmd->fdout;
		}
		else if (token->type == rin)
		{
			if (doc)
			{
				free(doc);
				doc = NULL;
			}
			cmd->fdin = open(token->next->str,  O_RDONLY);
			if (cmd->fdin < 0)
			{
				printf("Minishell: %s: No such file or directory\n", token->next->str);
				exfree(cmd, NULL, 'c', 1);
			}
			token->fd = cmd->fdin;
		}
		if (token->type == rdin)
		{
			if (doc)
				free(doc);
			doc = heredoc(token->next);
			token->fd = cmd->fdin;
		}
		token = token->next;
	}
	// if (doc)
	// {
	// 	if (pipe(pipfd) == -1)
	// 		return (NULL);
	// 	write(pipfd[1], doc, ft_strlen(doc));
	// 	dup2(pipfd[0], cmd->fdin);
	// 	// close(pipfd[1]);
	// 	// close(pipfd[0]);
	// 	free(doc);
	// }
	printf("HEREDOCSTR : %s\n", doc);
	return (doc);
}
