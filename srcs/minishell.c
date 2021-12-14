/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:52:17 by dasanter          #+#    #+#             */
/*   Updated: 2021/12/14 13:16:54 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    print_pars(t_pars *pars)
{
	int	i;

	printf("Parsing :\n");
	if (pars->line)
		printf("The command line: %s\n", pars->line);
	if (pars->pipe)
	{
		i = 0;
		while (pars->pipe)
		{
			printf("Pipe %d:%s\n", i, pars->pipe->arg->str);
			pars->pipe = pars->pipe->next;
			i++;
		}
	}
	if (pars->arg->str)
	{
		printf("Arg:%s\n", pars->arg->str);
	}
}

void    parsing(t_pars *pars, char *str)
{
	if (!check_line(str))
		exit_free(pars, "Error quote don't match");
	pars->line = str;
	pars->pipe = NULL;
	pars->arg = NULL;
	if (strchr(str, '|'))
		init_pipe(pars);
	else
		init_arg(pars);
}

void	loop(t_pars *pars)
{
	char *str;
	(void)pars;

	str = NULL;
	while (get_next_line(0, &str) > 0)
	{
		parsing(pars, str);
		print_pars(pars);
	}
}

int	main(int ac, char **av, char **env)
{
	int i;
	char **strenv;
	t_shell	shell;

	(void)ac;
	(void)av;
	i = -1;
	while (env[i])
		i++;
	strenv = malloc(sizeof(char *) * (i + 1));
	shell.pars = malloc(sizeof(t_pars));
	if (!shell.pars || !strenv)
	{
		printf("Error in pars malloc.\n");
		exit(EXIT_FAILURE);
	}
	i = -1;
	while (env[++i])
		strenv[i] = ft_strdup(env[i]);
	strenv[i] = 0;
	loop(shell.pars);
	return (1);
}