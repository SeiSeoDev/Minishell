/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 16:07:14 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/19 15:15:38 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_is_path(char *str)
{
	struct stat sb;
	if (stat(str, &sb) == -1)
		printf("ERREUR CA EXISTE PAS\n");
	return(0);
}

int	ex_cd(t_cmd *cmd)
{
	t_env   *myenv = NULL;
	char *old_pwd;

	printf("?\n");
	if (!cmd->arg->next || (cmd->arg->next->str && !ft_strcmp(cmd->arg->next->str, "~"))) // ICI
	{
		printf("get myenv:\n");
		myenv = handler(3, NULL, "HOME", NULL);
		old_pwd = handler(3, NULL, "PWD", NULL)->val;
		printf("%s=%s\n", myenv->name, myenv->val);

		handler(3, NULL, "PWD", myenv->val);
	}
	// else if ()
	// {
		
	// }
	printf("HOME :%s\n", myenv->val);
	return (0);
}

int	ex_pwd(t_cmd *cmd)
{
	t_env   *myenv;
	(void)cmd;
	myenv = handler(3, NULL, "PWD", NULL);
	write(1, myenv->val, ft_strlen(myenv->val));
	write(1, "\n", 1);
	return (0);
}