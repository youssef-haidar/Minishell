/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_command_builder1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalbrei <amalbrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 23:47:19 by yhaidar           #+#    #+#             */
/*   Updated: 2023/04/25 20:39:05 by amalbrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	edge_case(t_list *search)
{
	char	*line;

	line = (char *)search->content;
	if ((line[0] == '"' || line[0] == '\'') && is_meta_char(line[1])
		&& (line[2] == '"' || line[2] == '\'' || is_meta_char(line[2])))
		clean_quote((char **)&search->content);
}

void	commands_init(t_shell *data, int p_count)
{
	int	j;

	data->command = malloc(sizeof(t_command *) * (p_count + 1));
	j = -1;
	while (++j < p_count)
		data->command[j] = malloc(sizeof(t_command));
	data->command[j] = NULL;
}

/* Return 1 if String is Redirected */
int	is_str_redir(char *str)
{
	if (!ft_strncmp(str, ">", 2)
		|| !ft_strncmp(str, ">>", 3)
		|| !ft_strncmp(str, "<", 2)
		|| !ft_strncmp(str, "<<", 3))
		return (1);
	return (0);
}

int	count_pipes_in_lexar(t_list *lexar)
{
	t_list	*head;
	int		pipe_counter;

	head = lexar;
	pipe_counter = 1;
	while (head)
	{
		if (!ft_strncmp(head->content, "|", 2))
			pipe_counter++;
		head = head->next;
	}
	if (head)
	{
		ft_lstclear(&head, free);
		free(head);
	}
	return (pipe_counter);
}

/* Splits the Lexar List into Command List for executor */
int	split_into_commands(t_shell *data, t_list *lexar)
{
	int		i;
	int		p_count;

	i = 0;
	p_count = count_pipes_in_lexar(lexar);
	data->par->sections = ft_calloc(p_count + 1, sizeof(t_list *));
	while (lexar)
	{
		if (is_str_redir(lexar->content))
			ft_laddb(&data->par->sections[i], ft_ln(ft_strdup(lexar->content)));
		else if (!ft_strncmp(lexar->content, "|", 2))
		{
			ft_laddb(&data->par->pipe, ft_ln(ft_strdup(lexar->content)));
			i++;
		}
		else if (!is_str_redir(lexar->content))
			ft_laddb(&data->par->sections[i], ft_ln(ft_strdup(lexar->content)));
		lexar = lexar->next;
	}
	commands_init(data, p_count);
	i = -1;
	while (data->par->sections[++i])
		transfer_structs(data, data->par->sections[i], i, p_count);
	return (1);
}
