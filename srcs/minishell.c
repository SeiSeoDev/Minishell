/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:52:17 by dasanter          #+#    #+#             */
/*   Updated: 2021/12/15 14:09:22 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	loop(void)
{
	char *str;

	str = NULL;
	while (get_next_line(0, &str) > 0)
		split_words(str);
}

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
	if (!strenv)
		exit_free(NULL, "Error in env malloc.\n", 0);
	i = -1;
	while (env[++i])
		strenv[i] = ft_strdup(env[i]);
	strenv[i] = 0;
	loop();
	return (1);
}