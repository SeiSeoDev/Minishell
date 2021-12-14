/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:15:26 by tamigore          #+#    #+#             */
/*   Updated: 2021/12/08 16:20:25 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_cmd(char *res, char *str)
{
	int	i;

	i = 0;
	ft_bzero(res, ft_strlen(str));
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
		i++;
	while (str[i])
	{
		if (str[i] == '>' || str[i] == '<' || str[i] == '|' || \
			str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
			return (i);
		res[i] = str[i];
		i++;
	}
	return (i);
}

int	get_oper(char *res, char *str, int i)
{
	char	buf[3];
	int		j;
	int		tmp;

	buf[0] = '\0';
	buf[1] = '\0';
	buf[2] = '\0';
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
		i++;
	j = i;
	tmp = 0;
	while (str[j])
	{
		if (str[j] == '>' || str[j] == '<')
		{
			j++;
			tmp++;
			if (str[j] == '>' || str[j] == '<')
				j++;
		}
		j++;
	}
	if (tmp == 0)
		return (i);
	while (str[i])
	{
		if (str[i] == '>' || str[i] == '<')
		{
			buf[0] = str[i++];
			if (str[i] == str[i - 1])
				buf[1] = str[i++];
		}
		i++;
	}
	return (i);
}
