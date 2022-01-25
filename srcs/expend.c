/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expend.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 18:11:03 by tamigore          #+#    #+#             */
/*   Updated: 2022/01/25 17:49:13 by tamigore         ###   ########.fr       */
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
	if (i > 0)
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

char	*del_unused_quot(char *str)
{
	char	*res;
	char	quot;
	int		i;
	int		j;
	int		len;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			quot = str[i];
			j = i++;
			while (str[i] && str[i] != quot)
				i++;
			if (str[i])
			{
				while (j < i - 1)
				{
					str[j] = str[j + 1];
					j++;
				}
				printf("str:%s| j:%s| i:%s\n", str, &str[j], &str[i]);
				len = j - 1;
				str[j++] = str[++i]; 
				while (str[j])
					str[j++] = str[++i];
				str[j] = '\0';
				i = len;
			}
		}
		i++;
	}
	res = ft_strdup(str);
	free(str);
	return (res);
}

char	*expend_words(t_token *token, char *str)
{
	int		i;
	char	*util;
	int		j;
	t_env	*var;
	char	*res;

	i = 0;
	util = NULL;
	res = str;
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			j = i;
			if (quot_status(str, i) != 1)
			{
				if (ft_strncmp(str, "$?", 2) == 0)
					util = ft_strdup("$?");
				else
				{
					while (ft_isalnum(str[i]) || str[i] == '_')
						i++;
					util = ft_strndup(&str[j], i - j);
				}
				if (!util)
					exit_free(token, "Error ...\n",'t');
				var = handler(3, NULL, util, NULL);
				if (!var)
					res = replace_str(str, util, NULL);
				else
					res = replace_str(str, util, var->val);
				if (util)
					free(util);
				util = NULL;
			}
		}
		i++;
	}
	return (del_unused_quot(res));
}