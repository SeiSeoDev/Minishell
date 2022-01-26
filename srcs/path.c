/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 16:07:14 by dasanter          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/01/26 14:13:14 by dasanter         ###   ########.fr       */
=======
/*   Updated: 2022/01/25 12:39:55 by tamigore         ###   ########.fr       */
>>>>>>> 8b2c1e91cd45f91254b26ae63792cc010a96422c
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ex_cd(t_cmd *cmd)
{
	//t_env   *myenv = NULL;
	//char *old_pwd;
	char buf[4096];
	char *str;

<<<<<<< HEAD
	str = NULL;
	if (cmd->arg->next == NULL)
		str = ft_strdup(handler(3, NULL, "HOME", NULL)->val);
	else
		str = cmd->arg->next->str;
	printf("str : %s\n", str);
	if (!ft_strcmp(str, "~"))
		str = handler(3, NULL, "HOME", NULL)->val;
	else if (str[0] == '~' && str[1])
	{
		str = ft_strjoin(handler(3, NULL, "HOME", NULL)->val, "/");
		str = ft_strjoin(str, cmd->arg->next->str);
	}
	printf("OUAI : %s\n", str);
	if (chdir(str) == -1)
		return (0); // rajouter le msg d erreur
	else
	{
		handler(3, NULL, "OLDPWD", handler(3, NULL, "PWD", NULL)->val);
		handler(3, NULL, "PWD", getcwd(buf, 4096));
	}
	return (1);
=======
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
>>>>>>> 8b2c1e91cd45f91254b26ae63792cc010a96422c
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