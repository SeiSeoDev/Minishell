/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 13:44:02 by tamigore          #+#    #+#             */
/*   Updated: 2022/03/03 14:17:28 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_err(char *str, char *file)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(file, 2);
}

void	del_unquot_extra(char *str, int *i, int *j, char q)
{
	int	len;

	if (str[*j] == q && str[*i] == q)
	{
		while (str[*j])
		{
			str[*j] = str[*j + 1];
			(*j)++;
		}
		len = --(*i) - 1;
		while (str[*i])
		{
			str[*i] = str[*i + 1];
			(*i)++;
		}
		str[*i - 1] = '\0';
		*i = len;
	}
}

char	*del_unused_quot(char *str)
{
	char	*res;
	char	quot;
	int		i;
	int		j;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			quot = str[i];
			j = i++;
			while (str[i] && str[i] != quot)
				i++;
			del_unquot_extra(str, &i, &j, quot);
		}
		else
			i++;
	}
	res = ft_strdup(str);
	if (str)
		free(str);
	return (res);
}

void	print_token(t_token *token)
{
	int		i;
	t_token	*tmp;

	if (token)
	{
		i = 0;
		tmp = token;
		while (tmp)
		{
			printf("token [%d]: TYPE[%d] word=%s\n", i++, tmp->type, tmp->str);
			tmp = tmp->next;
		}
	}
	else
		printf("No token...\n");
}

void	print_cmd(t_cmd *cmd)
{
	int		i;
	t_cmd	*t;

	i = 0;
	t = cmd;
	while (t)
	{
		printf("CMD [%d]:\n", i++);
		if (t->arg)
		{
			printf("ARG: ");
			print_token(t->arg);
		}
		if (t->redir)
		{
			printf("REDIR: ");
			print_token(t->redir);
		}
		printf("in = %d | out = %d | pid = %d\n", t->fdin, t->fdout, t->pid);
		t = t->next;
	}
}
