/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:52:17 by dasanter          #+#    #+#             */
/*   Updated: 2022/02/17 11:13:51 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_add(t_token **tmp, t_cmd *data)
{
	t_token *use;

	if ((*tmp)->type == rdin || (*tmp)->type == rdout || (*tmp)->type == rin
		|| (*tmp)->type == rout)
	{
		if (!data->redir)
			data->redir = cmd_redir(tmp);
		else
		{
			use = data->redir;
			while (use->next)
				use = use->next;
			use->next = cmd_redir(tmp);
		}
	}
	else
	{
		if (!data->arg)
			data->arg = cmd_arg(tmp);
		else
		{
			use = data->arg;
			while (use->next)
				use = use->next;
			use->next = cmd_arg(tmp);
		}
	}
}

t_cmd	*cmd_init(t_cmd *res, t_token **tmp, t_token *token)
{
	t_token *use;
	t_cmd	*data;

	data = init_cmd(NULL, NULL, NULL);
	if (!data)
	{
		free_token(token);
		exfree(res, "init token failled\n", 'c', 1);
	}
	use = *tmp;
	*tmp = (*tmp)->next;
	use->next = NULL;
	free_token(use);
	return (data);
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
			ex_hit(NULL);
		}
		add_history(str);
		split_words(str);
		free(str);
	}
}

void sig_handler(int sig)
{
		printf("TEST\n");
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