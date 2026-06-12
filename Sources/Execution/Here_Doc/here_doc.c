/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caymard <caymard@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 16:04:30 by caymard           #+#    #+#             */
/*   Updated: 2024/05/17 13:34:28 by caymard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

void	get_here_doc(t_mini *shell, t_exec *exec)
{
	t_file	*file;

	while (exec)
	{
		file = exec->file;
		while (file)
		{
			if (file->type == HERE_DOC)
			{
				here_doc_exec(shell, file, exec);
				if (shell->status == 130)
					return ;
			}
			file = file->next;
		}
		exec = exec->next;
	}
}
