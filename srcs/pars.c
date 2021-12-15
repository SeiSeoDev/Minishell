/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 16:34:35 by tamigore          #+#    #+#             */
/*   Updated: 2021/12/15 18:15:31 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
void	cmd_creat(t_token *token)
{
	t_cmd	*data;
	t_cmd	*tmp;
	
	data = init_cmd(NULL, NULL, NULL);
	if (!data)
		exit_free(token, "Error init cmd\n", 't');
	while (token)
	{
		if (token->type == pip)
		{
			data->next = init_cmd(NULL, NULL, NULL);
			if (!data->next)
			{
				free_token(token);
				exit_free(tmp, "Error init cmd...\n",'c');
			}
			data = data->next;
		}
		token = token->next;
	}
	print_cmd(tmp);
}
*/
void	expension(t_token *token)
{
	t_token	*tmp;
	int		rd;

	tmp = token;
	rd = -1;
	while (tmp)
	{
		if (tmp->type == word)
			handle_words(token, tmp);
		// else if (tmp->type == fd)
		// {
		// 	handle_fd(token, tmp, rd);
		// 	rd = -1;
		// }
		else if (tmp->type == rin || tmp->type == rout || tmp->type == rdout)
			rd = tmp->type;
		tmp = tmp->next;
	}
	print_token(tmp);
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
	print_token(tmp);
	tokenize(tmp);
}
