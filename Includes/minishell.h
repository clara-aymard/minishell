/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 09:36:48 by caymard           #+#    #+#             */
/*   Updated: 2024/05/22 12:56:55 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

extern volatile sig_atomic_t	g_signal;

typedef enum s_token
{
	WORD = 0,
	VAR,
	S_QUOTE,
	D_QUOTE,
	PIPE,
	INPUT,
	HERE_DOC,
	TRUNC,
	APPEND
}	t_token;

typedef struct s_parse
{
	char			**paths;
	char			*path_cmd;
	int				fd_in;
	int				fd_out;
}	t_parse;

typedef struct s_file
{
	t_token			type;
	char			*file;
	int				quotted_heredoc;
	struct s_file	*next;
	int				hd_fd;
}	t_file;

typedef struct s_exec
{
	char			**av;
	t_file			*file;
	int				has_builtin;
	t_parse			parse;
	struct s_exec	*next;
	struct s_exec	*prev;
}	t_exec;

typedef struct s_lexer
{
	char			*str;
	t_token			token;
	struct s_lexer	*prev;
	struct s_lexer	*next;
}	t_lexer;

typedef struct s_expand_tools
{
	char	*input;
	char	**tab_var;
	int		nb_word;
	int		nb_delbslash;
	int		nb_delquote;
	int		in_squote;
	int		in_dquote;
	int		is_var;
	int		is_quote;
}	t_expand_tools;

typedef struct s_envp
{
	char			*name;
	char			*value;
	struct s_envp	*next;
}	t_envp;

typedef struct s_mini
{
	t_envp			*envp;
	char			**envv;
	int				status;
	t_lexer			*lex_list;
	t_exec			*exec;
	int				m_stdin;
	int				m_stdout;
}	t_mini;

// fonctions pour transformer les environnements
void	add_basic_vars(t_mini *shell);
void	check_basic_vars(t_mini *shell, t_envp *envp);
void	get_env_bash(char **env, t_mini *shell);
void	table_to_list(char **envv, t_mini *shell);
void	list_to_table(t_envp *envp, t_mini *shell);

//fonctions LIBFT
int		strlen_table(char **table);
int		ft_strlen(char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strchr(char *str, char c);
char	*ft_strdup(char *str);
char	**split(char const *str, char c);
char	*strjoin(char *str1, char *str2, char *c);

long	ft_atoi(char *str);
char	*ft_itoa(int n);

size_t	ft_strlcat(char *dst, char *src, size_t dstsize);
char	*ft_strnstr(char *haystack, char *needle, size_t len);
char	*ft_substr(char *str, unsigned int start, size_t len);
void	ft_bzero(void *str, size_t n);
int		ft_strcmp(char *s1, char *s2);

//fonctions pour remplacer printf
void	putstr_fd(int fd, char *str);
void	putendl_fd(int fd, char *str);
void	putmsg_fd(char *what, char *strerror, char *end, int fd);
void	error_msg(t_mini *shell, char *what, char *error, char *end);
void	hd_eof_warning(char *eof);

//fonctions UTILS - LIST
int		count_len_list(t_envp *envp);
int		is_alphanum(char c);
t_envp	*last_node(t_envp *envp);
void	add_last_var(t_envp **a, t_envp *new);
int		swap_var(t_mini *shell, t_envp *var1, t_envp *var2);

//fonction BUILTINS -- EXPORT
t_envp	*new_var(t_mini *shell, char *name, char *value, t_envp *next);
int		check_name(char *name);
int		check_exist(char *name, t_envp *envp);
void	add_var(t_mini *shell, t_envp *envp, char *name, char *value);
void	add_value(t_mini *shell, t_envp *envp, char *name, char *value);
void	replace_value(t_mini *shell, t_envp *envp, char *name, char *value);
void	print_export(t_mini *shell, t_envp *envp, int fd);
void	export(t_mini *shell, t_envp *envp, char **vars, int fd);

//fonctions BUILTINS -- CD
void	exec_cd(t_mini *shell, char *path);
void	get_var(t_mini *shell, t_envp *envp, char *var, t_envp **path);
void	exchange_pwd_oldpwd(t_mini *shell, t_envp *envp);
void	change_old_pwd(t_mini *shell, t_envp *envp, char *path);
void	cd(t_mini *shell, t_envp *envp, char **av);

// fonctions BUILTINS
void	echo(t_mini *shell, char **av, int fd);
void	env(t_mini *shell, t_envp *envp, char **av, int fd);
void	bash_exit(t_mini *shell, char **av);
void	pwd(t_mini *shell, int fd);
void	unset(t_mini *shell, t_envp *envp, char **vars);

// fonctions HERE_DOC
void	get_here_doc(t_mini *shell, t_exec *exec);
void	here_doc_exec(t_mini *shell, t_file *file, t_exec *exec);
void	hd_write_line(t_mini *shell, int quoted, int fd, char *line);
char	*hd_clean_delimiter(char *eof);
void	hd_fork_error(t_mini *shell, int *pipe_fd, char *eof);
int		open_hd_pipes(t_mini *shell, int *pipe_fd);

// EXECUTION/VAR_HEREDOC.C
char	*hd_var_expander(t_mini *shell, char *str);

// fonctions EXEC
void	check_file(t_mini *shell, t_exec *exec);
void	sort_execution(t_mini *shell, t_exec *exec);
void	pipe_check(t_mini *shell, t_exec *exec); //
void	wait_pipeline(t_mini *shell, pid_t pid); //UTILS
void	close_parent_fds(t_exec *exec, int *fd);
void	clean_child(t_mini *shell);
void	close_std_backup(t_mini *shell);

void	get_paths(t_mini *shell, char **envv, t_exec *exec, t_parse *parse);
void	get_builtin_alone(t_mini *shell, t_exec *exec);
void	exec_builtin(t_mini *shell, t_exec *exec, int out_fd);

//fonctions FREE / EXIT
void	free_table(char **table);
void	free_node_env(t_envp *node);
void	free_list_env(t_envp **envp);
void	free_list_file(t_file *file);
void	free_list_exec(t_exec **exec);
void	exit_minishell(t_mini *shell);
char	**free_malloc_split(char **ptr);

// EXPANDER.C
int		del_quotebslash_exe(t_expand_tools *e_tl, char *str, char *n, int *i);
void	init_exp_tools(t_expand_tools *exp_tools, char *input);
void	expander_exe(t_lexer **lex_list, t_mini *shell);
char	*del_quote_bslash(t_mini *shell, t_expand_tools *exp_tools, char *str);
char	*get_delquote(t_expand_tools *exp_tools, char *s);

// SIGNALS.C
int		av_init_signal(void);
void	handle_signal(int status);

// HANDLE_VAR.C
char	*expand_var(t_expand_tools *e_tl, int *i, t_mini *shell, char **str);
char	*var_expander(t_expand_tools *exp_tools, t_mini *shell);

//UTILS_EXPANDER.C
void	check_quotes(t_expand_tools *exp_tools, char *str, int *i, int flags);
int		condition_var(t_expand_tools *exp_tools, char *str, int i, int flag);
void	var_exe_nbword(t_expand_tools *exp_tools, char *str, int *i);
char	*get_word_var(t_expand_tools *e_tl, t_mini *shell, char *str, int *i);
int		cond_delquote_bslash(t_expand_tools *exp_tools, char *str, int i);

// LEXER.C
void	lexer_exe(char *str, t_lexer **lex_list, t_mini *shell);

// HANDLE_QUOTE.C
int		is_escaped(char *str, int index);
int		is_quote_formatted(char *str);
int		handle_quote(char *str, int i, int ch);

// HANDLE_TOKEN.C
int		is_token(char *str, int i);
int		handle_token(char *str, int i, t_token *token);

// LEXER_UTILS.C
int		ft_is_white_space(char c);

// LEXER_UTILS_LIST.C
t_lexer	*get_last_node_lex(t_lexer **lex_list);
void	print_lexlist(t_lexer **lex_list);
void	addnode_lexer(t_mini *shell, t_lexer **l_li, t_token token, char *str);
void	delete_lexlist(t_lexer **lex_list);

// PARSER_UTILS_FILELIST.C
void	delete_filelist(t_file **file_list);
int		addnode_flist(t_file **f_list, t_token token, char *file, int quote);

// PARSER_UTILS_EXECLIST.C
void	delete_execlist(t_exec **exec_list);
void	delnode_lexlist(t_lexer **exec_list, t_lexer *del);
void	addnode_execlist(t_mini *shell, t_exec **exec_list);

// PARSER_UTILS.C
int		is_quoted(char *str);

// PARSER.C
void	parser_exe(t_lexer **l_li, t_exec **e_li, t_mini *shell, int *n);

// TOOLS.C
char	*ft_strchr_premium(char *o_str, const char *p_str, int c, int *i);
char	*conc_strings(t_mini *shell, char **tab_strings);
int		is_special_char(char *str, char ch, int i);
int		is_redirect_token(t_token token);
void	minishell_exe(t_mini *shell);

// ERROR.C
void	ft_error_parser(int flag, t_mini *shell, char **tab_str);
void	minishell_loop(t_mini *shell);

#endif
