/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 16:34:35 by tamigore          #+#    #+#             */
/*   Updated: 2021/12/15 14:29:30 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
void	expension(t_token *token)
{
	t_cmd	*data;
	t_cmd	*tmp;
	int		i;
	int		f;
	t_token	*nul;

	data = malloc(sizeof(t_cmd));
	if (!data)
		exit_free(token, 't');
	tmp = data;
	f = 0;
	while (token)
	{
		if (token->type == pip)
		{
			data->next = malloc(sizeof(t_cmd));
			if (!data->next)
			{
				free_token(token);
				exit_free(data, "Error init cmd...\n",'c');
			}
			data = data->next;
			f = 1;
		}
		else if (token->type == word)
		{
			if (get_dquot())
			{
				token->str = get_env(token->str);
			}
		}
		else if (token->type == fd)
		{

		}
		token = token->next;
	}
	return (tmp);
}
*/
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
	print_token(token);
//	expension(token);
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
	print_token(tmp);
	tokenize(tmp);
}
