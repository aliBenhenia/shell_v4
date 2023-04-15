/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenheni <abenheni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 15:38:01 by mazaroua          #+#    #+#             */
/*   Updated: 2023/04/13 22:24:51 by abenheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *check_command_in_path(char **path, char *cmd)
{
	int i;

	i = 0;
	cmd = ft_strjoin("/", cmd);
	while (path[i])
	{
		if (!access(ft_strjoin(path[i], cmd), F_OK))
		{
			return (ft_strjoin(path[i], cmd));
			break;
		}
		i++;
	}
	return (NULL);
}

int count_envlist(t_env_list **env_list)
{
	t_env_list *envlist;
	int i;

	envlist = *env_list;
	i = 0;
	while (envlist)
	{
		i++;
		envlist = envlist->next;
	}
	return (i);
}

char **create_envp(t_env_list **env_list)
{
	t_env_list *envlist;
	char **envp;
	int i;
	char *tmp;

	envlist = *env_list;
	envp = malloc(sizeof(char *) * count_envlist(env_list) + 1);
	i = 0;
	while (envlist)
	{
		tmp = ft_strjoin(envlist->name, "=");
		envp[i] = ft_strjoin(tmp, envlist->value);
		envlist = envlist->next;
		i++;
	}
	return (envp);
}

void execve_func(char **cmd, t_env_list **env_list)
{
	char **path;
	char *cmd_;
	char **envp;

	path = get_path(env_list);
	cmd_ = check_command_in_path(path, cmd[0]);
	envp = create_envp(env_list);
	if (!cmd_)
	{
		perror("Command not found");
		exit(0);
	}
	execve(cmd_, cmd, envp);
}

void execute_command_2(t_cmd_line **cmd_line, t_env_list **env_list)
{
	int built_code;

	built_code = builtins((*cmd_line)->str[0]);
	if (built_code)
	{
		exec_builtins((*cmd_line)->str, env_list, built_code);
	}
	else
	{

		execve_func((*cmd_line)->str, env_list);
	}
}

void dup_to_pipe(int **fd, int i, int cmds)
{
	if (i == 0)
	{
		dup2(fd[i][1], 1);
		close(fd[i][0]);
		close(fd[i][1]);
	}
	else if (i + 1 == cmds)
	{
		dup2(fd[i - 1][0], 0);
		close(fd[i - 1][0]);
	}
	else if (i > 0 && i + 1 < cmds)
	{
		dup2(fd[i - 1][0], 0);
		dup2(fd[i][1], 1);
		close(fd[i - 1][0]);
		close(fd[i][1]);
		close(fd[i][0]);
	}
}

void	close_pipes(int **fd, int i, int cmds, int flag)
{
	if (flag != 1)
		return ;
	if (i == 0)
		close(fd[i][1]);
	else if (i + 1 == cmds)
		close(fd[i - 1][0]);
	else if (i > 0 && i + 1 < cmds)
	{
		close(fd[i][1]);
		close(fd[i - 1][0]);
	}
}

void execute_command(t_cmd_line **cmd_line, t_env_list **env_list, int **fd)
{
	t_cmd_line *cmd_tmp = *cmd_line;
	int no_file = 1;
	t_redirections	*l_infile = last_infile(cmd_line, &no_file);
	if (!no_file)
		return ;
	t_redirections	*l_outfile = last_outfile(cmd_line);
	int i = 0;
	int flg = 0;
	pid_t	pid;
	int	heredoc_fd;
	while (cmd_tmp)
	{
		if (cmd_tmp->separator == e_pipe)
		{
			pipe(fd[i]);
			flg = 1;
		}
		ft_heredoc(&cmd_tmp, &heredoc_fd);
		if (!(pid = fork()))
		{
			if (l_outfile)
				dup_outfile(l_outfile);
			if (l_infile)
				dup_infile(l_infile);
			if (flg)
				dup_to_pipe(fd, i, count_list(cmd_line));
			execute_command_2(&cmd_tmp, env_list);
		}
		close_pipes(fd , i, count_list(cmd_line), flg);
		cmd_tmp = cmd_tmp->next;
		i++;
	}
	while (wait(0) != -1);
}

void execution(t_cmd_line **cmd_line, t_env_list **env_list,t_export **export_list)
{
	if ((*cmd_line) && main_builtins(cmd_line) && (*cmd_line)->separator == e_nline)
	{
		execute_builtins(cmd_line, env_list,export_list);
	}
	else if ((*cmd_line))
	{
		int **fd = malloc(sizeof(int *) * count_list(cmd_line));
		int i = 0;
		while (i < count_list(cmd_line))
			fd[i++] = malloc(sizeof(int) * 2);
		execute_command(cmd_line, env_list, fd);
	}
}