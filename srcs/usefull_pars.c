/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usefull_pars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:51:36 by tamigore          #+#    #+#             */
/*   Updated: 2022/02/25 18:37:53 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	quot_status(char *str, int i)
{
	int		j;
	char	quot;

	j = 0;
	quot = 0;
	while (str[j] && j < i)
	{
		if (str[j] == '"' || str[j] == '\'')
		{
			if (quot == 0)
				quot = str[j];
			else if (quot == str[j])
				quot = 0;
		}
		j++;
	}
	if (quot == '\'')
		return (1);
	else if (quot == '"')
		return (2);
	else
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
		while (str[*i] && !(str[*i] == '|' || str[*i] == '<' || str[*i] == '>' ||
			str[*i] == ' ' || str[*i] == '\t'))
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

t_token	*token_syntax(t_token *token)
{
	t_token	*tmp;
	int		ret;

	tmp = token;
	ret = 0;
	if (!token)
		return (NULL);
	if (tmp->type == pip)
		ret = 1;
	while (tmp && ret == 0)
	{
		if ((tmp->type == pip && !tmp->next) ||
			(tmp->type == pip && tmp->next && tmp->next->type == pip) ||
			(tmp->type >= 3 && tmp->next && tmp->next->type >= pip))
			ret = 1;
		if (!tmp->next)
			break ;
		tmp = tmp->next;
	}
	if (ret == 1)
	{
		printf("Minishell: syntax error near unexpected token: %s\n", tmp->str);
		ctrfree(token, NULL, 't', 2);
		return (NULL);
	}
	return (token);
}
