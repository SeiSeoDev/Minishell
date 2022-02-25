/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:52:17 by dasanter          #+#    #+#             */
/*   Updated: 2022/02/25 14:50:53 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_file(char *path)
{
	struct stat	sb;
	int			res;

	if (!path)
		return (0);
	if (lstat(path, &sb) == -1)
		return (0);
	res = (sb.st_mode & S_IFMT);
	if (res == S_IFBLK)
		return (1);
	else if (res == S_IFCHR)
		return (2);
	else if (res == S_IFDIR)
		return (3);
	else if (res == S_IFIFO)
		return (4);
	else if (res == S_IFLNK)
		return (5);
	else if (res == S_IFREG)
		return (6);
	else if (res == S_IFSOCK)
		return (7);
	else
		return (8);
}

void	loop(void)
{
	char 	*str;

	str = NULL;
	while (1)
	{
		str = readline("\e[1m\e[31m\002""Minishell : ""\001\e[0m\002");
		// printf("%s\n", str);
		if (str == NULL)
		{
			printf("exit\n");
			exfree(NULL, NULL, 0, 1);
		}
		add_history(str);
		split_words(str);
		free(str);
	}
}

void sig_handler(int sig)
{
	printf("Quit (core dumped)\n");
	if (sig == SIGINT) 
	{
		rl_replace_line("", 0);
		printf("\nIS PARENT\n");
		rl_on_new_line();
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		ft_putstr_fd("\b\b  \b\b", 1);
		return;
	}
}

int	main(int ac, char **av, char **env)
{
	// t_env	*myenv;
	char	*str;

	(void)ac;
	(void)av;
	handler(0, env, NULL, NULL);
	str = ft_itoa(ft_atoi(handler(3, NULL, "SHLVL", NULL)->val) + 1);
	handler(3, NULL, "SHLVL", str);
	free(str);
/*
**	myenv = handler(3, NULL, NULL, NULL);
**	while (myenv)
**	{
**		printf("%s=%s\n", myenv->name, myenv->val);
**		myenv = myenv->next;
**	}
*/
	printf("PID : %d\n", getpid());
	signal(SIGINT, sig_handler2);
	signal(42, sig_handler2);
	signal(SIGQUIT, sig_handler2);
	loop();
	return (1);
}