/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:52:17 by dasanter          #+#    #+#             */
/*   Updated: 2022/03/02 15:35:21 by dasanter         ###   ########.fr       */
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

void	sig_heredoc(int sig)
{
	rl_on_new_line();
	(void)sig;
	ft_putchar_fd('\n', 1);
	exit(0);
}

int	get_nbpipe(t_cmd *cmd)
{
	int		i;
	t_cmd	*tmp;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

void	loop(void)
{
	char	*str;

	str = NULL;
	while (1)
	{
		str = readline("\e[1m\e[31m\002""Minishell : ""\001\e[0m\002");
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

int	main(int ac, char **av, char **env)
{
	char	*str;

	(void)ac;
	(void)av;
	handler(0, env, NULL, NULL);
	str = ft_itoa(ft_atoi(handler(3, NULL, "SHLVL", NULL)->val) + 1);
	handler(3, NULL, "SHLVL", str);
	free(str);
	signal(SIGINT, sig_handler);
	signal(42, sig_handler);
	signal(SIGQUIT, sig_handler);
	loop();
	return (1);
}
