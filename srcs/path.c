/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 16:07:14 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/25 12:39:55 by tamigore         ###   ########.fr       */
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

	if (!cmd->arg->next || (cmd->arg->next->str && !ft_strcmp(cmd->arg->next->str, "~"))) // ICI
	{
		printf("get myenv:\n");
		myenv = handler(3, NULL, "HOME", NULL);
		old_pwd = handler(3, NULL, "PWD", NULL)->val;
		printf("OLD PWD %s\n", old_pwd);
		printf("%s=%s\n", myenv->name, myenv->val);
		handler(3, NULL, "PWD", myenv->val);
		myenv = handler(3, NULL, "PWD", NULL);
		printf("PWD :%s\n", myenv->val);
	}
	// else if ()
	// {
		
	// }
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