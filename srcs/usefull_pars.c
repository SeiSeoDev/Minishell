/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usefull_pars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:51:36 by tamigore          #+#    #+#             */
/*   Updated: 2021/12/14 18:40:56 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_sep(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == ' ' || c == '\t')
		return (1);
	return (0);
}

void	skip_cot(char *str, int *i)
{
	char	cot;

	cot = str[*i];
	(*i)++;
	while (str[*i])
	{
		if (str[*i] == cot)
		{
			(*i)++;
			break ;
		}
		(*i)++;
	}
}

void	split(char *str, int *i, int *last)
{
	while (str[*i] == ' ' || str[*i] == '\t')
		(*i)++;
	*last = *i;
	if (ft_strncmp(&str[*i], "<<", 2) == 0 || ft_strncmp(&str[*i], ">>", 2) == 0)
		(*i) += 2;
	else if (str[*i] == '<' || str[*i] == '>' || str[*i] == '|')
		(*i)++;
	else
	{
		while (str[*i] && !check_sep(str[*i]))
		{
			if (str[*i] == '"' || str[*i] == '\'')
				skip_cot(str, i);
			else
				(*i)++;
		}
	}
}

void	get_type(t_token *tmp, int *l, int *f)
{
	if (ft_strcmp(tmp->str, ">>") == 0)
		tmp->type = rdout;
	else if (ft_strcmp(tmp->str, "<<") == 0)
		tmp->type = rdin;
	else if (ft_strcmp(tmp->str, ">") == 0)
		tmp->type = rout;
	else if (ft_strcmp(tmp->str, "<") == 0)
		tmp->type = rin;
	else if (ft_strcmp(tmp->str, "|") == 0)
		tmp->type = pip;
	else
	{
		// if l == 1 && fd == 1 ???
		if (*l == 1)
		{
			tmp->type = lim;
			*l = 0;
		}
		else if (*f == 1)
		{
			tmp->type = fd;
			*f = 0;
		}
		else
			tmp->type = word;
	}
}
