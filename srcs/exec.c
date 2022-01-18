/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 15:50:00 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/18 15:58:03 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <time.h>

// static int	exe_cmd(t_cmd *cmd)
// {
// 	t_env	*env;
// 	t_cmd	*tmp;
// 	char	**arg;
// 	char	*exe;
// 	int		i;

// 	env = handler(3, NULL, "PATH", NULL);
// 	if (!env)
// 		return (0);
// 	tmp = cmd;
// 	i = 0;
// 	while (tmp->arg)
// 	{
// 		tmp->arg = tmp->arg->next;
// 		i++;
// 	}
// 	arg = malloc(sizeof(char *) * (i + 1));
// 	if (!arg)
// 		return (0);
// 	tmp = cmd;
// 	i = 0;
// 	while (tmp->arg)
// 	{
// 		arg[i++] = tmp->arg->str;
// 		tmp->arg = tmp->arg->next;
// 	}
// 	if 	(execve(exe, arg, env->all) == -1)
// 	{
// 		printf("command failed\n");
// 	}
// 	else
// 	{
// 		printf("command success\n");
// 	}
// 	return (0);
// }

// static int  find_file(char *path)
// {
// 	struct stat	sb;
// 	int			res;

// 	if (path)
// 		printf("%s <pathname>\n", path);
// 	if (stat(path, &sb) == -1)
// 	{
// 		perror("stat");
// 		exit(EXIT_SUCCESS);
// 	}
// 	printf("Type de fichier : ");
// 	res = (sb.st_mode & S_IFMT);
// 	if (res == S_IFBLK)
// 			printf("périphérique de bloc\n");
// 	else if (res == S_IFCHR)
// 			printf("périphérique de caractère\n");
// 	else if (res == S_IFDIR)
// 			printf("répertoire\n");
// 	else if (res == S_IFIFO)
// 			printf("FIFO/tube\n");
// 	else if (res == S_IFLNK)
// 			printf("lien symbolique\n");
// 	else if (res == S_IFREG)
// 			printf("fichier ordinaire\n");
// 	else if (res == S_IFSOCK)
// 			printf("socket\n");
// 	else
// 	{
// 		printf("inconnu ?\n");
// 		return (0);
// 	}
// 	printf("Numéro d'inœud :            %ld\n", (long)sb.st_ino);
// 	printf("Mode :                      %lo (octal)\n", (unsigned long)sb.st_mode);
// 	printf("Nombre de liens :           %ld\n", (long)sb.st_nlink);
// 	printf("Propriétaires :                    UID=%ld   GID=%ld\n", (long)sb.st_uid, (long)sb.st_gid);
// 	printf("Taille de bloc d’E/S :      %ld octets\n", (long)sb.st_blksize);
// 	printf("Taille du fichier :         %lld octets\n", (long long)sb.st_size);
// 	printf("Blocs alloués :             %lld\n", (long long)sb.st_blocks);
// 	printf("Dernier changement d’état : %s", ctime(&sb.st_ctime));
// 	printf("Dernier accès au fichier :  %s", ctime(&sb.st_atime));
// 	printf("Dernière modif du fichier:  %s", ctime(&sb.st_mtime));
// 	return (1);
// }

void    exec(t_cmd *cmd)
{
	//printf("first\n");
	if (cmd != NULL && cmd->arg != NULL)
	{
		//printf("TEST : %s\n", cmd->arg->str);
		if (!ft_strcmp(cmd->arg->str, "echo"))
			ex_echo(cmd);
		else if (!ft_strcmp(cmd->arg->str, "cd"))
			ex_cd(cmd);
		else if (!ft_strcmp(cmd->arg->str, "pwd"))
			ex_pwd(cmd);
		else
		{
			// if (!find_cmd(cmd->arg->str))
				printf("Minishell: %s: command not found\n", cmd->arg->str);
		}
	}

	/*
	fork
	if (child)
		exec
	if (parent)
		cmd -> next;
	*/
}