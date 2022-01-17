/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expend.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 18:11:03 by tamigore          #+#    #+#             */
/*   Updated: 2022/01/13 17:28:05 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*replace_str(char *str, char *old, char *new)
{
	char	*res;
	int		i;
	int		j;
	int		k;

	if (!new || !old)
		return (NULL);
	j = ft_strlen(old) + 1;
	res = malloc(sizeof(char) * ((ft_strlen(str) - j) + (ft_strlen(new) - j)) + 1);
	if (!res)
		return (NULL);
	i = 0;
	k = 0;
	while (str[k] && ft_strncmp(&str[i], old, ft_strlen(old)))
		res[i++] = str[k++];
	i--;
	while (new[j])
		res[i++] = new[j++];
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
				tmp->str = replace_str(tmp->str, util, handler(1, NULL, util));
				free(util);
			}
		}
		i++;
	}
}

// void	expend_fd(t_token *token, t_token *tmp, int rd)
// {
// 	int		i;
// 	char	*util;
// 	int		quot;
// 	int		j;

// 	i = 0;
// 	if (rd == rin)
// 	{
//     	j = open(tmp->str, );
// 		if (j < 0)
// 			exit_free(token, "open failed", 't');
// 	}
// 	else if (rd == rout)
// 	{
// 		j = open(tmp->str, );
// 		if (j < 0)
// 			exit_free(token, "open failed", 't');
// 	}
// 	else if (rd == rdout)
// 	{
// 		j = open(tmp->str, O_WRONLY | );
// 		if (j < 0)
// 			exit_free(token, "open failed", 't');
// 	}
// }