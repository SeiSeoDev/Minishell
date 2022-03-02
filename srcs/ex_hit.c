/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_hit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 17:23:41 by dasanter          #+#    #+#             */
/*   Updated: 2022/03/02 18:25:17 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ex_hit(t_cmd  *cmd)
{
    t_token *arg;

    arg = cmd->arg;
    if (arg && arg->next)
        arg = arg->next;
    else
        return (0);
    if (ft_strlen(arg->str) > 19 || ft_superatoi(arg->str) < 0 || !ft_strisnum(arg->str))
    {
        print_err("numeric argument required ", arg->str);
        exfree(cmd, "\nexit", 'c', 2);
    }
    return (ft_superatoi(arg->str) % 256);
}