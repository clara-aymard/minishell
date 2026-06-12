/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 13:47:43 by caymard           #+#    #+#             */
/*   Updated: 2024/05/17 15:07:37 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

volatile sig_atomic_t	g_signal;

int	av_init_signal(void)
{
	struct termios	term;

	handle_signal(1);
	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return (0);
	term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		return (0);
	return (1);
}

static void	sig_handler(int signal)
{
	g_signal = signal;
	if (signal == SIGINT)
		putendl_fd(2, "^C");
}

static void	sigint_int(int signal)
{
	g_signal = signal;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	write(1, "^C\n", 3);
	rl_on_new_line();
	rl_redisplay();
}

static void	sigint_child_hd(int signal)
{
	g_signal = signal;
	write(1, rl_prompt, ft_strlen(rl_prompt));
	write(1, rl_line_buffer, ft_strlen(rl_line_buffer));
	write(1, "^C\n", 3);
	exit(130);
}

void	handle_signal(int status)
{
	if (status == 1)
		return ((void)signal(SIGQUIT, SIG_IGN),
			(void)signal(SIGINT, sigint_int));
	if (status == 2)
		return ((void)signal(SIGINT, sig_handler),
			(void)signal(SIGQUIT, sig_handler));
	if (status == 3)
		return ((void)signal(SIGQUIT, SIG_IGN),
			(void)signal(SIGINT, SIG_IGN));
	if (status == 4)
		return ((void)signal(SIGQUIT, SIG_IGN),
			(void)signal(SIGINT, sigint_child_hd));
}
