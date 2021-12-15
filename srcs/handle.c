/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 18:11:03 by tamigore          #+#    #+#             */
/*   Updated: 2021/12/15 18:15:03 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*replace_str(char *str, char *old, char *new)
{
	char	*res;
	int		i;
	int		j;
	int		k;

	if (!new)
		return (NULL);	
	res = malloc(sizeof(char) * (ft_strlen(str) - ft_strlen(old) + ft_strlen(new)));
	if (!res)
		return (NULL);
	i = 0;
	k = 0;
	while (str[i] && ft_strncmp(str, old, ft_strlen(old)) == 0)
		res[i++] = str[k++];
	j = 0;
	while (new[j])
		res[i++] = new[j++];
	k += ft_strlen(old);
	while (str[k])
		res[i++] = str[k++];
	res[i] = '\0';
	return (res);
}

void	handle_words(t_token *token, t_token *tmp)
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
			j = i;
			quot = quot_status(tmp->str, i);
			if (quot == 2 || quot == 0)
			{
				while (ft_isalnum(tmp->str[i]) || tmp->str[i] == '_')
					i++;
				util = ft_strndup(&tmp->str[j], i - j);
				printf("env var = %s\n", util);
				if (!util)
					exit_free(token, "Error ...\n",'t');
				tmp->str = replace_str(tmp->str, util, getenv(util));
			}
		}
		i++;
	}
}

// void	handle_fd(t_token *token, t_token *tmp, int rd)
// {
// 	int		i;
// 	char	*util;
// 	int		quot;
// 	int		j;

// 	i = 0;
// 	if (rd == rin)
//     {
//         open(tmp->str);
//         if 
//     }
// }