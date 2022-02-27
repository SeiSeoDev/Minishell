/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 16:34:35 by tamigore          #+#    #+#             */
/*   Updated: 2022/02/27 14:15:38 by user42           ###   ########.fr       */
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
	if (!parsing_error(res))
		return ;
	print_cmd(res);
	child(res);
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
				if (tmp->str[i] == '$' && quot_status(tmp->str, i) != 1 &&
					(ft_isalnum(tmp->str[i + 1]) || tmp->str[i + 1] == '_' ||
					tmp->str[i + 1] == '?' || tmp->str[i + 1] == '$'))
				{
					tmp->str = expend_words(tmp->str, i);
					printf("expend = %s\n", tmp->str);
				}
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
			{
				ctrfree(tmp, "error init token...\n", 't', 1);
				return ;
			}
		}
	}
	print_token(tmp);
	tokenize(tmp);
}
