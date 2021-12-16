/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:52:17 by dasanter          #+#    #+#             */
/*   Updated: 2021/12/16 18:36:58 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	loop(void)
{
	char *str;

	str = NULL;
	while (1)
	{
		str = readline("\e[1m\e[31m\002""Minishell : ""\001\e[0m\002");
		add_history(str);
		//split_words(str);
	}
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
	//test_exec();
	loop();
	return (1);
}