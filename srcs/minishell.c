/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:52:17 by dasanter          #+#    #+#             */
/*   Updated: 2021/12/14 15:23:37 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_separator(char *str/*, char *cot*/)
{
	if (ft_strncmp(str, "<<", 2) == 0 || ft_strncmp(str, ">>", 2) == 0)
		return (2);
	else if (str[0] == '<' || str[0] == '>' || str[0] == ' ' || str[0] == '|')
		return (1);
	else if (str[0] == '\0')
		return (3);
	return (0);
}

void	skip_cot(char *str, int *i)
{
	char	cot;

	cot = str[*i];
	(*i)++;
	while (str[*i])
	{
		if (str[*i] == cot)
			break ;
		(*i)++;
	}
}

void	tokenize(t_token *token)
{
	(void)token;
}

void    split_words(char *str)
{
	t_token	*token;
	t_token *tmp;
	int		i;
	int		last;
	int		ret;

	i = 0;
	token = NULL;
	while (str[i])
	{
		last = i;
		ret = 0;
		while (ret == 0)
		{
			if (str[i] == '"' || str[i] == '\'')
				skip_cot(str, &i);
			ret = is_separator(&str[i]);
			i++;
		}
		printf("str[last] = %s\n", &str[last]);
		if (!token)
		{
			token = init_token(NULL, ft_strndup(&str[last], i - (last + 1 )), 0);
			tmp = token;
		}
		else
		{
			token->next = init_token(NULL, ft_strndup(&str[last], i - (last + 1)), 0);
			token = token->next;
		}
		if (!token)
			exit_free(NULL, "error init token...");
		if (ret == 3)
			break ;
		i += ret - 1;
	}
	printf("Have I succed ?\n");
	print_token(tmp);
	tokenize(tmp);
}

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
	{
		printf("Error in pars malloc.\n");
		exit(EXIT_FAILURE);
	}
	i = -1;
	while (env[++i])
		strenv[i] = ft_strdup(env[i]);
	strenv[i] = 0;
	loop();
	return (1);
}