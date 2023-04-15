/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenheni <abenheni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 14:07:54 by mazaroua          #+#    #+#             */
/*   Updated: 2023/04/14 00:30:26 by abenheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	print_env(t_env_list *env)
{
	t_env_list *head = (env);
	while (head)
	{
		printf("%s=\"%s\"\n",head->name,head->value);
		head = head->next;
	}
}
int	builtins(char	*cmd)
{

	if (!ft_strcmp(cmd, "cd"))
		return (2);
	if (!ft_strcmp(cmd, "pwd"))
		return (3);
	return (0);
}

void	exec_builtins(char	**str, t_env_list **env_list, int code)
{
	if (code == 2)
    {
		ft_cd(str[1], env_list);
        exit(0);
    }
	else if (code == 3)
    {
		ft_pwd();
        exit(0);
    }
	
}

int main_builtins(t_cmd_line **cmd_line)
{
    if ((*cmd_line) && (*cmd_line)->str[0])
    {
        if (!ft_strcmp((*cmd_line)->str[0], "cd"))
            return (1);
        if (!ft_strcmp((*cmd_line)->str[0], "export"))
            return (1);
        if (!ft_strcmp((*cmd_line)->str[0], "env"))
            return (1);
        if (!ft_strcmp((*cmd_line)->str[0], "unset"))
            return (1);
        if (!ft_strcmp((*cmd_line)->str[0], "echo"))
            return (1);
    }
    return (0);
}

void    execute_builtins(t_cmd_line **cmd_line, t_env_list **env_list,t_export **export_list)
{
    if (!ft_strcmp((*cmd_line)->str[0], "cd"))
        ft_cd((*cmd_line)->str[1], env_list);
    if (!ft_strcmp((*cmd_line)->str[0], "pwd"))
        ft_pwd();
    if (!ft_strcmp((*cmd_line)->str[0], "export"))
       do_export((*cmd_line)->str,export_list,env_list);
    if (!ft_strcmp((*cmd_line)->str[0], "env"))
    {
        t_env_list *head = (*env_list);
        while (head)
        {
            if(ft_strcmp(head->value,"") == 0)
                printf("%s\n",head->name);
            else
                printf("%s=\"%s\"\n",head->name,head->value);
            head = head->next;
        }
    }
    if (!ft_strcmp((*cmd_line)->str[0], "unset"))
        do_unset((*cmd_line)->str,export_list,env_list);
    if (!ft_strcmp((*cmd_line)->str[0], "echo"))
        handle_echo(*cmd_line);
}