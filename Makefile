CC			  = cc
CFLAGS		  = -Wall -Wextra -Werror
READLINE_FLAGS  = -lreadline

NAME			= minishell

GREEN		   = \033[0;32m
RED			 = \033[0;31m
RESET		   = \033[0m

MAIN_SRC		= Sources/main.c \
				  Sources/prompt.c 

CD_SRC		  = Sources/Builtins/cd/cd.c \
				  Sources/Builtins/cd/cd_utils.c

EXPORT_SRC	  = Sources/Builtins/export/export.c \
				  Sources/Builtins/export/export_utils.c \
				  Sources/Builtins/export/export_utils_2.c \
				  Sources/Builtins/export/print_export.c \
				  Sources/Builtins/export/print_export_utils.c

BUILTINS_SRC	= $(CD_SRC) \
				  Sources/Builtins/echo.c \
				  Sources/Builtins/env.c \
				  Sources/Builtins/exit.c \
				  $(EXPORT_SRC) \
				  Sources/Builtins/pwd.c \
				  Sources/Builtins/unset.c

EXECUTION_SRC   = Sources/Execution/execution.c \
				  Sources/Execution/infile_outfile.c \
				  Sources/Execution/parsing.c \
				  Sources/Execution/pipe.c \
				  Sources/Execution/pipe_utils.c \
				  Sources/Execution/Here_Doc/here_doc.c \
				  Sources/Execution/Here_Doc/here_doc_exec.c \
				  Sources/Execution/Here_Doc/here_doc_utils.c \
				  Sources/Execution/Here_Doc/var_heredoc.c

SIGNALS_SRC	 = Sources/Signals/signals.c

ENV_SRC		 = Sources/Utils/env/env_utils.c \
				  Sources/Utils/env/switching_env.c

UTILS_SRC	   = $(ENV_SRC) \
				  Sources/Utils/atoi_itoa.c \
				  Sources/Utils/free_exit.c \
				  Sources/Utils/libft.c \
				  Sources/Utils/libft_2.c \
				  Sources/Utils/lst_fct.c \
				  Sources/Utils/split.c \
				  Sources/Utils/strjoin.c \
				  Sources/Utils/write_functions.c \
				  Sources/Utils/tools.c \
				  Sources/Utils/error_exit.c

EXP_SRC		 = Sources/expander/expander.c \
				  Sources/expander/handle_var.c \
				  Sources/expander/utils_expander.c

LEX_SRC		 = Sources/lexer/lexer.c \
				  Sources/lexer/lexer_utils.c \
				  Sources/lexer/lexer_utils_list.c \
				  Sources/lexer/handle_quote.c \
				  Sources/lexer/handle_token.c

PAR_SRC		 = Sources/parser/parser.c \
				  Sources/parser/parser_utils_execlist.c \
				  Sources/parser/parser_utils_filelist.c \
				  Sources/parser/parser_utils.c

SRCS			= $(MAIN_SRC) \
				  $(UTILS_SRC) \
				  $(BUILTINS_SRC) \
				  $(EXECUTION_SRC) \
				  $(SIGNALS_SRC) \
				  $(EXP_SRC) \
				  $(LEX_SRC) \
				  $(PAR_SRC)

OBJS			= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(READLINE_FLAGS)
	@printf "$(GREEN)✔ $(NAME) built$(RESET)\n"

.c.o:
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@printf "$(RED)🗑 Objects removed$(RESET)\n"

fclean: clean
	@rm -f $(NAME)
	@printf "$(RED)🗑 $(NAME) removed$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re