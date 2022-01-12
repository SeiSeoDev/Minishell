/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 16:34:35 by tamigore          #+#    #+#             */
/*   Updated: 2022/01/12 19:12:16 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_creat(t_token *token)
{
	t_cmd	*data;
	t_cmd	*res;
	t_token	*tmp;
	t_token *s_arg;
	t_token *s_redir;
	int		redir;
	int		fa;
	int		fr;

	data = init_cmd(NULL, NULL, NULL);
	if (!data)
		exit_free(token, "Error init cmd\n", 't');
	res = data;
	tmp = token;
	s_redir = NULL;
	s_arg = NULL;
	redir = 0;
	fa = 0;
	fr = 0;
	while (token)
	{
		if (token->type == pip)
		{
			redir = 0;
			fa = 0;
			fr = 0;
			data->arg = s_arg;
			data->redir = s_redir;
			data->next = init_cmd(NULL, NULL, NULL);
			if (!data->next)
			{
				free_token(token);
				exit_free(res, "Error init_cmd...\n",'c');
			}
			data = data->next;
		}
		else if (token->type == word || token->type == lim || token->type == fd)
		{
			if (redir == 0)
			{
				data->arg = init_token(NULL, token->str, token->type);
				print_token(data->arg);
			}
			else
			{
				data->redir = init_token(NULL, token->str, token->type);
				print_token(data->redir);
			}
		}
		else if (token->type == rout || token->type == rin || token->type == rdout || token->type == rdin)
		{
			redir = 1;
			data->redir = init_token(NULL, token->str, token->type);
			print_token(data->redir);
		}
		if (fa == 0 && data->arg)
		{
			s_arg = data->arg;
			fa = 1;
		}
		if (fr == 0 && data->redir)
		{
			s_redir = data->redir;
			fr = 1;
		}
		if (data->arg)
			data->arg = data->arg->next;
		if (data->redir)
			data->redir = data->redir->next;
		token = token->next;
	}
	data->arg = s_arg;
	data->redir = s_redir;
	printf("arg:\n");
	print_token(s_arg);
	printf("redir:\n");
	print_token(s_redir);
	free_token(tmp);
	printf("Cmd creat:\n");
	print_cmd(res);
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
	printf("Expension:\n");
	print_token(token);
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
	printf("Tokenize:\n");
	print_token(token);
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
	printf("Split words:\n");
	print_token(tmp);
	tokenize(tmp);
}
