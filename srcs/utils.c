/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 16:47:15 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/17 14:57:43 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env *get_env(t_env *env, char *search)
{
    int i;
    t_env *ret;

    ret = env;
    i = 0;
    while (ret)
    {
        printf("%s\n", ret->str);
        while (search[i] && ret->str[i] && search[i] == ret->str[i])
            i++;
        if (i == (int)ft_strlen(search) && search[i - 1] == ret->str[i - 1])
            return (ret);
        ret = ret->next;
    }
    return (NULL);
}

char    *get_value(t_env *env, char *name)
{
    t_env *this;
    char *ret;
    this = get_env(env, name);
    ret = this->str;
    while (ret != NULL && ret && *ret != '=')
    {
        ret++;
    }
    if (!ret)
        return (NULL);
    ret++;
    return (ft_strdup(ret));
}

char    *join_env(char *name, char *value)
{
    int i;
    int j;
    char *str;

    i = 0;
    j = 0;
    str = malloc((ft_strlen(name) + ft_strlen(value) + 2) * sizeof(char));
    while (name[i])
    {
        str[i] = name[i];
        i++;
    }
    str[i++] = '=';
    while (value[j])
    {
        str[i + j] = value[j];
        j++;
    }
    str[i + j] = 0;
    return (str);
}

void	edit_env(t_env *env, char *name, char *new_value)
{
	t_env *this;

    this = get_env(env, name);
    printf("AFTER GET\n : %s\n", this->str);
    if (!this)
        return;
    printf("test\n");
   // free(this->str); IL VA FALLOIR FAIRE CA UN JOUR
    this->str = join_env(name, new_value);
    printf("AFTER JOIN\n : %s\n", this->str);

}

void	edit_path(t_env *env, char *new_path)
{
    edit_env(env, "PWD", new_path);
}