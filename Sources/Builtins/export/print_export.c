/* ************************************************************************** */
/*							                                                */
/*                                                        :::      ::::::::   */
/*   print_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:21:26 by caymard           #+#    #+#             */
/*   Updated: 2024/04/04 20:21:29 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

int	check_swap_var(t_mini *shell, t_envp **head, t_envp *var1, t_envp *var2)
{
	t_envp	*temp;

	temp = *head;
	if (*head == var1)
	{
		if (swap_var(shell, var1, var2) == 1)
			return (1);
	}
	else
	{
		while (temp->next != var1)
			temp = temp->next;
		if (swap_var(shell, var1, var2) == 1)
			return (1);
	}
	return (0);
}

static t_envp	*copy_list(t_mini *shell, t_envp *envp)
{
	t_envp	*copy;

	if (!envp)
		return (NULL);
	copy = new_var(shell, envp->name, envp->value,
			copy_list(shell, envp->next));
	if (!copy)
		return (NULL);
	return (copy);
}

static int	check_ascii_order(t_mini *shell, t_envp *envp, t_envp **sorted)
{
	t_envp	*temp;
	int		len;
	int		count;

	*sorted = copy_list(shell, envp);
	if (!*sorted)
		return (1);
	count = count_len_list(*sorted);
	while (count)
	{
		temp = *sorted;
		while (temp && temp->next)
		{
			len = ft_strlen(temp->name);
			if (temp->next && (len < ft_strlen(temp->next->name)))
				len = ft_strlen(temp->next->name);
			if (ft_strcmp(temp->name, temp->next->name) > 0)
				if (check_swap_var(shell, sorted, temp, temp->next) == 1)
					return (1);
			temp = temp->next;
		}
		count--;
	}
	return (0);
}

void	print_export(t_mini *shell, t_envp *envp, int fd)
{
	t_envp	*temp;
	t_envp	*sorted;

	if (check_ascii_order(shell, envp, &temp) == 1 || !temp)
	{
		free_list_env(&temp);
		exit_minishell(shell);
	}
	sorted = temp;
	while (sorted)
	{
		putstr_fd(fd, "declare -x ");
		putstr_fd(fd, sorted->name);
		if (!sorted->value)
			putstr_fd(fd, "\n");
		else if (sorted->name && sorted->value)
		{
			putmsg_fd("=\"", sorted->value, "\"", fd);
		}
		sorted = sorted->next;
	}
	free_list_env(&temp);
	shell->status = 0;
}
