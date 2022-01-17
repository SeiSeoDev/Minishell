/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 16:07:14 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/17 19:43:05 by tamigore         ###   ########.fr       */
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

	printf("?\n");
	if (!cmd->arg->next || (cmd->arg->next->str && !ft_strcmp(cmd->arg->next->str, "~"))) // ICI
	{
		printf("get myenv:\n");
		myenv = handler(3, NULL, "HOME", NULL);
		printf("%s=%s\n", myenv->name, myenv->val);
		check_is_path(myenv->val);
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