/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalbrei <amalbrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 02:22:39 by yhaidar           #+#    #+#             */
/*   Updated: 2023/04/24 18:35:25 by amalbrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	msh_eof(t_shell *data)
{
	int	exit_code;

	printf("exit\n");
	msh_free(&data->command);
	msh_free_list(&data->env);
	msh_free_list(&data->dec_env);
	msh_free(&data->oldpwd);
	msh_free(&data->par);
	exit_code = data->exit_code;
	msh_free(&data);
	exit (exit_code);
}

void	msh_reset(t_shell *data)
{
	int	i;

	if (g_stdin == -2)
		data->exit_code = 130;
	if (g_stdin == -3)
		data->exit_code = 1;
	if (g_stdin == -4)
		data->exit_code = 131;
	g_stdin = -1;
	i = -1;
	if (data->command)
	{
		while (data->command[++i])
		{
			msh_complete_close(data, data->command[i]);
			msh_array_free(data, i);
		}
		msh_free(&data->command);
	}
	data->par->error = 0;
	data->nohd = 0;
}

/* Initialises All Data */
t_shell	*initialiser(char **env)
{
	t_shell	*data;

	g_stdin = -1;
	data = ft_calloc(1, sizeof(t_shell));
	data->par = ft_calloc(1, sizeof(t_parser));
	data->par->error = 0;
	data->exit_code = 0;
	data->nohd = 0;
	data->oldpwd = NULL;
	data->line = NULL;
	if (env[0])
	{
		msh_create_env(&data, env);
		msh_create_denv(&data, env);
		msh_update_shlvl(data->env);
		msh_update_shlvl(data->dec_env);
	}
	else
	{
		data->env = NULL;
		data->dec_env = NULL;
	}
	return (data);
}

int	main(int argc, char **argv, char **env)
{
	t_shell		*data;

	(void)argc;
	(void)argv;
	data = initialiser(env);
	signal(SIGINT, signal_handler_parent);
	signal(SIGQUIT, signal_handler_parent);
	while (1)
	{
		data->line = ft_strtrim(readline("\033[1;33mminishell:B> \033[0m"),
				" \v\t\f\r\n");
		if (data->line == NULL)
			break ;
		if (data->line[0] == 0)
		{
			msh_free(&data->line);
			continue ;
		}
		add_history(data->line);
		if (parser(data, &data->line))
			msh_command_dispenser(data);
		msh_reset(data);
	}
	msh_eof(data);
}
