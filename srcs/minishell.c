/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:52:17 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/19 17:14:01 by tamigore         ###   ########.fr       */
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
		printf("%s\n", str);
		add_history(str);
		split_words(str);
	}
}

void sig_handler(int sig)
{
	if (sig == SIGINT) 
		printf("\nCTRL + C need to free stucts\n ");
	else if (sig == SIGQUIT)
		printf("CTRL + -\\ need to do nothing only catch");
	//else
	exit(EXIT_SUCCESS);
}

int	main(int ac, char **av, char **env)
{
	t_env	*myenv;

	(void)ac;
	(void)av;
	handler(0, env, NULL, NULL);
	myenv = handler(3, NULL, NULL, NULL);
	while (myenv)
	{
		printf("%s=%s\n", myenv->name, myenv->val);
		myenv = myenv->next;
	}
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	loop();
	return (1);
}