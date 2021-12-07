/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:52:17 by dasanter          #+#    #+#             */
/*   Updated: 2021/12/07 19:00:48 by tamigore         ###   ########.fr       */
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
		printf("The initial line received in argument: %s\n", pars->line);
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
	if (pars->pipe)
	{
		i = 0;
		while (pars->pipe)
		{
			printf("Pipe %d:%s\n", i, pars->pipe->arg.str);
			pars->pipe = pars->pipe->next;
			i++;
		}
	}
	if (pars->arg->str)
		printf("Arg:%s\n", pars->arg->str);
}

void	init_pipe(t_pars *pars)
{
	(void)pars;
}

void	init_arg(t_pars *pars)
{
	int	i;
	int	j;
	char **tmp;

	i = 0;
	if (pars->split)
	{
		while (pars->split[i])
			i++;
	}
	pars->arg = malloc(sizeof(t_arg) * i);
	i = 0;
	while (pars->split[i])
	{
		pars->arg[i].str = pars->split[i];
		tmp = ft_split(pars->arg[i].str, '\"');
		printf("start:\n");
		j = 0;
		while (tmp[j])
		{
			printf("%s\n", tmp[j]);
			j++;
		}
		pars->arg[i].split = ft_split(pars->arg[i].str, ' ');
		i++;
	}
}

void    parsing(t_pars *pars, char *str)
{
	int	i;

	pars->line = str;
	printf("error\n");
	pars->split = ft_split(str, ';');
	printf("error 2\n");
	if (!pars->split)
		exit_free(pars);
	pars->pipe = NULL;
	pars->arg = NULL;
	i = 0;
	while (pars->split[i])
	{
		if (ft_strchr(str, '|'))
			init_pipe(pars);
		else
			init_arg(pars);
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