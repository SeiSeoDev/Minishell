/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 16:34:35 by tamigore          #+#    #+#             */
/*   Updated: 2022/03/02 18:55:41 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec(t_cmd *cmd, char *doc)
{
	if (cmd && cmd->redir)
		fill_fd(cmd, doc);
	if (isntopen(cmd) == 1)
		return (close_fd(cmd));
	if (!cmd || !cmd->arg || !cmd->arg->str)
		exfree(cmd, NULL, 'c', 1);
	dup2(cmd->fdin, STDIN_FILENO);
	if (!ft_strcmp(cmd->arg->str, "echo"))
		return (ex_echo(cmd));
	else if (!ft_strcmp(cmd->arg->str, "cd"))
		return (ex_cd(cmd, NULL, handler(3, NULL, "HOME", NULL), 0));
	else if (!ft_strcmp(cmd->arg->str, "pwd"))
		return (ex_pwd(cmd));
	else if (!ft_strcmp(cmd->arg->str, "env"))
		return (ex_env(cmd));
	else if (!ft_strcmp(cmd->arg->str, "unset"))
		return (ex_unset(cmd));
	else if (!ft_strcmp(cmd->arg->str, "export"))
		return (ex_port(cmd));
	else if (!ft_strcmp(cmd->arg->str, "exit"))
		exfree(cmd, "exit", 'c', ex_hit(cmd));
	dup2(cmd->fdout, STDOUT_FILENO);
	exe_cmd(cmd);
	close_fd(cmd);
}

void	cmd_creat(t_token *token)
{
	t_cmd	*data;
	t_cmd	*res;
	t_token	*tmp;

	if (!token)
		return ;
	data = init_cmd(NULL, NULL, NULL);
	if (!data)
		exfree(token, "Error init cmd\n", 't', 1);
	res = data;
	tmp = token;
	while (tmp)
	{
		if (tmp->type == pip)
		{
			data->next = cmd_init(res, &tmp, token);
			data = data->next;
		}
		else
			cmd_add(&tmp, data);
	}
	if (!res || !parsing_error(res))
		return ;
	parent(res);
}

void	expension(t_token *token)
{
	t_token	*tmp;
	int		i;

	tmp = token;
	while (tmp)
	{
		if (tmp->type == word || tmp->type == fd)
		{
			i = 0;
			while (tmp->str && tmp->str[i])
			{
				if (tmp->str[i] == '$' && quot_status(tmp->str, i) != 1
					&& (ft_isalnum(tmp->str[i + 1]) || tmp->str[i + 1] == '_'
						|| tmp->str[i + 1] == '?' || tmp->str[i + 1] == '$'))
					tmp->str = expend_words(tmp->str, i);
				else
					tmp->str = del_unused_quot(tmp->str);
				i++;
			}
		}
		tmp = tmp->next;
	}
	cmd_creat(token);
}

void	tokenize(t_token *token)
{
	t_token	*tmp;
	int		l;
	int		f;

	tmp = token;
	l = 0;
	f = 0;
	while (tmp)
	{
		get_type(tmp, &l, &f);
		if (tmp->type == rdout || tmp->type == rout || tmp->type == rin)
			f = 1;
		else if (tmp->type == rdin)
			l = 1;
		tmp = tmp->next;
	}
	if (!token_syntax(token))
		return ;
	expension(token);
}

void	split_words(char *str, int i, int s)
{
	t_token	*token;
	t_token	*tmp;

	if (!str)
		return ;
	if (!str[0])
		return (free(str));
	split(str, &i, &s);
	token = init_token(NULL, ft_strndup(&str[s], i - s), 0);
	tmp = token;
	while (str[i])
	{
		split(str, &i, &s);
		if (i > s)
		{
			token->next = init_token(NULL, ft_strndup(&str[s], i - s), 0);
			token = token->next;
			if (!token)
				ctrfree(tmp, "error init token...\n", 't', 1);
		}
	}
	ft_memdel((void **)&str);
	tokenize(tmp);
}
