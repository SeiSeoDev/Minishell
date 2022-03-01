/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 16:34:35 by tamigore          #+#    #+#             */
/*   Updated: 2022/02/25 17:39:19 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_creat(t_token *token)
{
	t_cmd	*data;
	t_cmd	*res;
	t_token	*tmp;

	if (!token)
		child(NULL);
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
	res = parsing_error(res);
	if (!res)
		return ;
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
		{
			tmp->str = expend_words(tmp->str);
			if (!tmp->str || !ft_strcmp(tmp->str, ""))
				del_token(&token, tmp);
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
	token = token_syntax(token);
	expension(token);
}

void	split_words(char *str)
{
	t_token	*token;
	t_token	*tmp;
	int		i;
	int		last;

	i = 0;
	last = 0;
	tmp = NULL;
	token = NULL;
	while (str[i])
	{
		split(str, &i, &last);
		if (i > last)
		{
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
				exfree(token, "error init token...\n", 't', 1);
		}
	}
//	print_token(tmp);
	tokenize(tmp);
}
