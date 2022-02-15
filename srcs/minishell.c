/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:52:17 by dasanter          #+#    #+#             */
/*   Updated: 2022/02/09 14:23:22 by dasanter         ###   ########.fr       */
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
		// printf("%s\n", str);
		if (str == NULL)
		{
			ft_putchar_fd('\n', 1);
			loop();
		}
		add_history(str);
		split_words(str);
	}
}

void sig_handler(int sig)
{
	if (sig == SIGINT) 
	{
		rl_replace_line("", 0);
		printf("\nIS PARENT\n");
		rl_on_new_line();
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		printf("CTRL + -\\ need to do nothing only catch");
		exit(EXIT_SUCCESS);
	}
}

int	main(int ac, char **av, char **env)
{
	t_env	*myenv;
	char	*str;

	(void)ac;
	(void)av;
	handler(0, env, NULL, NULL);
	str = ft_itoa(ft_atoi(handler(3, NULL, "SHLVL", NULL)->val) + 1);
	myenv = handler(3, NULL, "SHLVL", str);
	free(str);
	myenv = handler(3, NULL, NULL, NULL);
	while (myenv)
	{
		printf("%s=%s\n", myenv->name, myenv->val);
		myenv = myenv->next;
	}
	printf("PID : %d\n", getpid());
	signal(SIGINT, sig_handler2);
	signal(SIGQUIT, sig_handler);
	loop();
	return (1);
}