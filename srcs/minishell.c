/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:52:17 by dasanter          #+#    #+#             */
/*   Updated: 2021/12/08 13:18:27 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_free(t_pars *pars)
{
	t_pipe	*tmp;
	int		i;

	if (pars)
	{
		if (pars->split)
		{
			i = 0;
			while (pars->split[i])
				free(pars->split[i++]);
			free(pars->split);
		}
		if (pars->pipe)
		{
			while (pars->pipe)
			{
				tmp = pars->pipe;
				pars->pipe = pars->pipe->next;
				free(tmp);
			}
		}
		if (pars->arg)
		{
			if (pars->arg->split)
			{
				i = 0;
				while (pars->arg->split[i])
					free(pars->arg->split[i++]);
				free(pars->arg->split);
			}
			free(pars->arg);
		}
	}
}

void    print_pars(t_pars *pars)
{
	int	i;

	printf("Parsing :\n");
	if (pars->line)
		printf("The command line: %s\n", pars->line);
	if (pars->split)
	{
		i = 0;
		printf("Split:\n");
		while (pars->split[i])
		{
			printf("[%d]:%s\n", i, pars->split[i]);
			i++;
		}
	}
	// if (pars->pipe)
	// {
	// 	i = 0;
	// 	while (pars->pipe)
	// 	{
	// 		printf("Pipe %d:%s\n", i, pars->pipe->arg.str);
	// 		pars->pipe = pars->pipe->next;
	// 		i++;
	// 	}
	// }
	// if (pars->arg->str)
	// 	printf("Arg:%s\n", pars->arg->str);
}

void	init_pipe(t_pars *pars)
{
	(void)pars;
}

void	init_arg(t_pars *pars, int i)
{
	int	i;
	char **tmp;

	i = 0;
	pars->arg = malloc(sizeof(t_arg));
	if (!pars->arg)
		exit_free(pars);
	pars->arg->str = pars->split[i];
	pars->arg->split = ft_split(pars->arg[i].str, ' ');
	pars->arg->cmd = pars->arg[i].split[0];
	while 
	i++;
}

int		check_line(char *str)
{
	int	i;
	int	sq;
	int	dq;
	int	lock;

	i = 0;
	sq = 0;
	dq = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			if (sq == 0)
				
		}
	}
}

void    parsing(t_pars *pars, char *str)
{
	int	i;

	if (!check_line(str))
		exit_free(pars, "Error quote don't match");
	pars->line = str;
	pars->split = ft_split(str, ';');
	if (!pars->split)
		exit_free(pars);
	pars->pipe = NULL;
	pars->arg = NULL;
	i = 0;
	while (pars->split[i])
	{
		init_arg(pars, i);
		i++;
	}
}

int	main(int ac, char **av, char **env)
{
	int i;
	char **strenv;
	t_pars  *pars;

	i = -1;
	while (env[i])
		i++;
	strenv = malloc(sizeof(char *) * (i + 1));
	i = -1;
	while (env[++i])
		strenv[i] = ft_strdup(env[i]);
	strenv[i] = 0;
	if (ac == 2)
	{
		pars = malloc(sizeof(t_pars));
		if (!pars)
		{
			printf("Error in pars malloc.\n");
			exit(EXIT_FAILURE);
		}
		parsing(pars, av[1]);
		print_pars(pars);
	}
	return (1);
}