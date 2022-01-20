/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expend.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 18:11:03 by tamigore          #+#    #+#             */
/*   Updated: 2022/01/20 16:07:02 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *cmd_arg(t_token **tmp)
{
	t_token *res;
	t_token *stop;

	res = *tmp;
	if (*tmp)
	{
		if ((*tmp)->type == pip)
			return (NULL);
		while ((*tmp)->next && (*tmp)->next->type != pip && (*tmp)->type != pip &&
			   (*tmp)->next->type != rin && (*tmp)->next->type != rout &&
			   (*tmp)->next->type != rdin && (*tmp)->next->type != rdout)
			(*tmp) = (*tmp)->next;
	}
	stop = res;
	while (stop != *tmp)
		stop = stop->next;
	if (*tmp)
		*tmp = (*tmp)->next;
	if (stop)
		stop->next = NULL;
	return (res);
}

t_token *cmd_redir(t_token **tmp)
{
	t_token *res;
	t_token *stop;

	res = *tmp;
	if (*tmp)
	{
		if ((*tmp)->type == pip)
			return (NULL);
		while ((*tmp)->next && (*tmp)->type != pip && (*tmp)->next->type != pip)
			(*tmp) = (*tmp)->next;
	}
	stop = res;
	while (stop != *tmp)
		stop = stop->next;
	if (*tmp)
		*tmp = (*tmp)->next;
	if (stop)
		stop->next = NULL;
	return (res);
}

static char	*replace_str(char *str, char *old, char *new)
{
	char	*res;
	int		i;
	int		j;
	int		k;

	j = ft_strlen(old) + 1;
	res = malloc(sizeof(char) * ((ft_strlen(str) - j) + (ft_strlen(new) + 1)));
	if (!res)
		return (NULL);
	i = 0;
	k = 0;
	while (str[k] && ft_strncmp(&str[i], old, ft_strlen(old)))
		res[i++] = str[k++];
	i--;
	if (new)
	{
		j = 0;
		while (new[j])
			res[i++] = new[j++];
	}
	k += ft_strlen(old);
	while (str[k])
		res[i++] = str[k++];
	res[i] = '\0';
	free(str);
	return (res);
}

void	expend_words(t_token *token, t_token *tmp)
{
	int		i;
	char	*util;
	int		quot;
	int		j;
	t_env	*var;

	i = 0;
	while (tmp->str[i])
	{
		if (tmp->str[i] == '$')
		{
			i++;
			j = i;
			quot = quot_status(tmp->str, i);
			if (quot == 2 || quot == 0)
			{
				if (ft_strncmp(tmp->str, "$?", 2) == 0)
					util = ft_strdup("$?");
				else
				{
					while (ft_isalnum(tmp->str[i]) || tmp->str[i] == '_')
						i++;
					util = ft_strndup(&tmp->str[j], i - j);
				}
				if (!util)
					exit_free(token, "Error ...\n",'t');
				var = handler(3, NULL, util, NULL);
				// printf("util=%s\n", util);
				if (!var)
					tmp->str = replace_str(tmp->str, util, NULL);
				else
					tmp->str = replace_str(tmp->str, util, var->val);
				free(util);
			}
		}
		i++;
	}
}