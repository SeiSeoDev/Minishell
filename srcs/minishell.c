/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:52:17 by dasanter          #+#    #+#             */
/*   Updated: 2021/12/07 16:08:31 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int	main(int ac, char **av, char **env)
{
	int i;
	char **strenv;
	
	(void)ac;
	(void)av;

	i = -1;
	while (env[i])
		i++;
	strenv = malloc(sizeof(char *) * (i + 1));
	i = -1;
	while (env[++i])
		strenv[i] = ft_strdup(env[i]);
	strenv[i] = 0;
	return (1);
}