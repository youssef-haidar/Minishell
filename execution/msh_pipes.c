/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_pipes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalbrei <amalbrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 13:23:05 by amalbrei          #+#    #+#             */
/*   Updated: 2023/04/27 11:31:29 by amalbrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	msh_check_redir(t_shell *shell, int i)
{
	int	check_failed;

	check_failed = 0;
	if (shell->command[i]->redir)
	{
		msh_redirect(shell, shell->command[i], shell->command[i]->redir);
		if (shell->command[i]->fd_in == -1 || shell->command[i]->fd_out == -1)
			check_failed = 1;
	}
	if (check_failed)
		return (1);
	return (0);
}

void	msh_create_pipe(t_shell *shell, t_command *command, int tmp_fd)
{
	if (pipe(command->p_fd) < 0)
	{
		msh_print_error(shell, command, strerror(errno), 1);
		exit (shell->exit_code);
	}
	command->fd_in = tmp_fd;
	command->fd_out = command->p_fd[1];
}

void	msh_pipe_command(t_shell *shell, t_command *command, int *tmp_fd, int i)
{
	msh_create_pipe(shell, command, *tmp_fd);
	if (msh_redirect(shell, command, command->redir))
	{
		msh_check_command_piped(shell, command, *tmp_fd, i);
		msh_protected_close(command->p_fd[1], -1, -2);
		msh_protected_close(*tmp_fd, -1, -2);
		*tmp_fd = dup(command->p_fd[0]);
		msh_protected_close(command->p_fd[0], -1, -2);
	}
	else
	{
		msh_protected_close(command->p_fd[1], -1, -2);
		msh_protected_close(*tmp_fd, -1, -2);
		*tmp_fd = dup(command->p_fd[0]);
		msh_protected_close(command->p_fd[0], -1, -2);
		command->pid = fork();
		if (command->pid == 0)
		{
			msh_protected_close(*tmp_fd, -1, -2);
			msh_free_to_exit(shell);
		}
	}
}

void	msh_last_command(t_shell *shell, t_command *command, int tmp_fd, int i)
{
	command->fd_in = tmp_fd;
	command->fd_out = STDOUT_FILENO;
	if (msh_redirect(shell, command, command->redir))
	{
		msh_check_command_piped(shell, command, tmp_fd, i);
		msh_protected_close(tmp_fd, -1, -2);
	}
	else
	{
		msh_protected_close(tmp_fd, -1, -2);
		command->pid = fork();
		if (command->pid == 0)
			msh_free_to_exit(shell);
	}
}
