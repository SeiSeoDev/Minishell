/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 16:34:35 by tamigore          #+#    #+#             */
/*   Updated: 2022/01/13 17:33:14 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*cmd_arg(t_token **tmp)
{
	t_token *res;
	t_token *arg;

	res = NULL;
	arg = NULL;
	while ((*tmp) && (*tmp)->type != pip && (*tmp)->type != rin && (*tmp)->type != rout &&
		(*tmp)->type != rdin && (*tmp)->type != rdout)
	{
		if (arg == NULL)
		{
			arg = init_token(NULL, ft_strdup((*tmp)->str), (*tmp)->type);
			if (!arg)
				return (NULL);
			res = arg;
		}
		else
		{
			arg->next = init_token(NULL, ft_strdup((*tmp)->str), (*tmp)->type);
			if (!arg->next)
				return (NULL);
			arg = arg->next;
		}
		(*tmp) = (*tmp)->next;
	}
	return (res);
}

t_token	*cmd_redir(t_token **tmp)
{
	t_token *res;
	t_token *redir;

	res = NULL;
	redir = NULL;
	while ((*tmp) && (*tmp)->type != pip)
	{
		if (redir == NULL)
		{
			redir = init_token(NULL, ft_strdup((*tmp)->str), (*tmp)->type);
			if (!redir)
				return (NULL);
			res = redir;
			printf("redir str:%s\n", redir->str);
		}
		else
		{
			redir->next = init_token(NULL, ft_strdup((*tmp)->str), (*tmp)->type);
			if (!redir->next)
				return (NULL);
			redir = redir->next;
			printf("redir str:%s\n", redir->str);
		}
		(*tmp) = (*tmp)->next;
	}
	return (res);
}

void	cmd_creat(t_token *token)
{
	t_cmd	*data;
	t_cmd	*res;
	t_token	*tmp;

	data = init_cmd(NULL, NULL, NULL);
	if (!data)
		exit_free(token, "Error init cmd\n", 't');
	res = data;
	tmp = token;
	while (tmp)
	{
		if (tmp->type == pip)
		{
			data->next = init_cmd(NULL, NULL, NULL);
			if (!data->next)
			{
				free_token(token);
				exit_free(res, "Error init_cmd...\n",'c');
			}
			data = data->next;
			tmp = tmp->next;
		}
		else
		{
			printf("Before cmd arg\n");
			print_token(tmp);
			data->arg = cmd_arg(&tmp);
			printf("Before cmd redir\n");
			print_token(tmp);
			data->redir = cmd_redir(&tmp);
			printf("After cmd redir\n");
			print_token(tmp);
		}
	}
	/*printf("token before free\n");
	print_token(token);
	free_token(token);
	printf("Cmd creat:\n");
	print_cmd(res);*/
	exec(res);
}

void	expension(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp)
	{
		if (tmp->type == word || tmp->type == fd)
			expend_words(token, tmp);
		tmp = tmp->next;
	}
	// printf("Expension:\n");
	// print_token(token);
	cmd_creat(token);
}

void	tokenize(t_token *token)
{
	t_token *tmp;
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
	// printf("Tokenize:\n");
	// print_token(token);
	expension(token);
}

void	split_words(char *str)
{
	t_token	*token;
	t_token *tmp;
	int		i;
	int		last;

	i = 0;
	tmp = NULL;
	token = NULL;
	while (str[i])
	{
		split(str, &i, &last);
		if (i == last)
			continue ;
		if (!token)
		{
			token = init_token(NULL, ft_strndup(&str[last], i - last), 0);
			tmp = token;
		}
		else
		{
			token->next = init_token(NULL, ft_strndup(&str[last], i - last), 0);
			token = token->next;
		}
		if (!token)
			exit_free(token, "error init token...\n", 't');
	}
	// printf("Split words:\n");
	// print_token(tmp);
	tokenize(tmp);
}
