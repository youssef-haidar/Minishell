/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalbrei <amalbrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 18:49:50 by amalbrei          #+#    #+#             */
/*   Updated: 2023/04/16 14:42:19 by amalbrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

/*-------- msh_change.c --------*/
void	msh_update_shlvl(t_env *env);
void	msh_remove_node(t_env **env, t_env *target);
void	msh_create_node(t_env *env, char *variable, char *value);
void	msh_update_dec_env(t_env *env, char *variable, char *value);
void	msh_update_env(t_env *env, char *variable, char *value);

/*-------- msh_create.c --------*/
char	*msh_quotes(char *value);
t_env	*msh_create_first(char **env, int *i);
void	msh_create_rest(t_env **head, char **env, int *i);
void	msh_create_denv(t_shell **shell, char **env);
void	msh_create_env(t_shell **shell, char **env);

/*-------- msh_empty.c --------*/
void	msh_update_dec(t_shell **shell, char *mixed);
void	msh_create_new_env(t_shell **shell, char *mixed);
void	msh_create_new_d_env(t_shell **shell, char *mixed);
void	msh_empty(t_shell **shell, t_command *command, char *mixed);

/*-------- msh_ex.c --------*/
void	msh_update_env_ex(t_env *env, char *variable, char *value);
void	msh_update_secluded(t_env *env, char *variable);
t_env	*msh_find_node_ex(t_env *env, char *line);
char	*msh_find_env_ex(t_env *env, char *line);

/*-------- msh_find.c --------*/
t_env	*msh_find_last_node(t_env *env);
t_env	*msh_find_node(t_env *env, char *line);
char	*msh_find_env(t_env *env, char *line);

#endif