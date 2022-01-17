/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 16:07:14 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/17 16:34:44 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_is_path(char *str)
{
    struct stat sb;
    if (stat(str, &sb) == -1)
        printf("ERREUR CA EXISTE PAS\n");
    return(0);
}

int ex_cd(t_cmd *cmd)
{
    (void)cmd;
    char *home = NULL;

    if (!cmd->arg->next || (cmd->arg->next->str && !ft_strcmp(cmd->arg->next->str, "~"))) // ICI
    {
        home = handler(7, NULL, "HOME");
        check_is_path(home);
        handler(6, NULL, home);
    }
	// else if ()
	// {
		
	// }
    printf("HOME :%s\n", home);
    return (0);
}

int ex_pwd(t_cmd *cmd)
{
    char *path;
    (void)cmd;
    path = handler(7, NULL, "PWD");
    write(1, path, ft_strlen(path));
    write(1, "\n", 1);
    return (0);
}