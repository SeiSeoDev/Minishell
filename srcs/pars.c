/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 16:34:35 by tamigore          #+#    #+#             */
/*   Updated: 2022/01/28 16:04:49 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_creat(t_token *token)
{
	t_cmd	*data;
	t_cmd	*res;
	t_token	*tmp;
	t_token	*tofree;

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
			tofree = tmp;
			tmp = tmp->next;
			tofree->next = NULL;
			free_token(tofree);
		}
		else
		{
			if (tmp->type == rdin || tmp->type == rdout || tmp->type == rin || tmp->type == rout)
			{
				if (!data->redir)
					data->redir = cmd_redir(&tmp);
				else
				{
					tofree = data->redir;
					while (tofree->next)
						tofree = tofree->next;
					tofree->next = cmd_redir(&tmp);
				}
			}
			else
			{
				if (!data->arg)
					data->arg = cmd_arg(&tmp);
				else
				{
					tofree = data->arg;
					while (tofree->next)
						tofree = tofree->next;
					tofree->next = cmd_arg(&tmp);
				}
			}
		}
	}
	parsing_error(res);
	print_cmd(res);
	child(res);
}

void	expension(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp)
	{
		if (tmp->type == word || tmp->type == fd)
			tmp->str = expend_words(tmp, tmp->str);
		tmp = tmp->next;
	}
	// print_token(token);
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
	token_syntax(token);
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
	tokenize(tmp);
}
