/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:52:17 by dasanter          #+#    #+#             */
/*   Updated: 2022/02/21 14:30:48 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_file(char *path)
{
	struct stat	sb;
	// int			res;

	if (!path)
		return (0);
	if (lstat(path, &sb) == -1)
		return (0);
	// printf("Type de fichier : ");
	// res = (sb.st_mode & S_IFMT);
	// if (res == S_IFBLK)
	// 	printf("périphérique de bloc\n");
	// else if (res == S_IFCHR)
	// 	printf("périphérique de caractère\n");
	// else if (res == S_IFDIR)
	// 	printf("répertoire\n");
	// else if (res == S_IFIFO)
	// 	printf("FIFO/tube\n");
	// else if (res == S_IFLNK)
	// 	printf("lien symbolique\n");
	// else if (res == S_IFREG)
	// 	printf("fichier ordinaire\n");
	// else if (res == S_IFSOCK)
	// 	printf("socket\n");
	// else
	// 	printf("inconnu ?\n");
	return (1);
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
<<<<<<< HEAD
	printf("Quit (core dumped)\n");
	if (sig == SIGINT) 
=======
	printf("TEST\n");
	if (sig == SIGINT)
>>>>>>> 572d9fe4ee62079e143d075cb9c2a247cb7993bb
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
	signal(SIGQUIT, sig_handler2);
	loop();
	return (1);
}