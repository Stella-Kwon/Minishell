# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/30 12:38:40 by skwon2            #+#    #+#              #
#    Updated: 2024/10/24 22:17:38 by skwon2           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

INCLUDE_DIR = ./includes/

RM = rm -rf

LIBFT = ./libft/libft.a

FLAG = -Wall -Wextra -Werror -I $(INCLUDE_DIR) # -g -fsanitize=address

COMPILE_FLAG = -I/opt/homebrew/opt/readline/include/

LINK_FLAG = -lreadline -lncurses -L/opt/homebrew/opt/readline/lib

#LINK_FLAG = -lreadline -lncurses -L/


SRCS_DIR = ./srcs/

SRCS =	mini.c \
		mini_utils.c \
		utils/ft_strcmp.c\
		utils/ft_strndup.c\
		utils/ft_realloc.c\
		utils/ft_isspace.c\
		utils/free_one.c\
		utils/all_free.c\
		utils/ft_strcpy.c\
		utils/ft_strcat.c\
		utils/ft_strjoin3.c\
		utils/waitpid_status.c\
		tokenize/store_words.c\
		tokenize/readline_again.c\
		tokenize/check_set.c\
		tokenize/tokenize.c\
		tokenize/tokenize_operator.c\
		tokenize/operation_error.c\
		tokenize/operation_next.c\
		tokenize/operation_next_utils.c\
		tokenize/handle_tokens.c\
		tokenize/handle_redirection.c\
		tokenize/input_utils.c\
		signal/signal.c\
		redirection/heredoc.c\
		redirection/heredoc_utils.c\
		redirection/herestring.c\
		parsing/create_astnode.c\
		parsing/create_command.c\
		parsing/initialize_func.c\
		parsing/free_func.c\
		parsing/astnode_utils.c\
		parsing/heredoc_herestr_parsing.c\
		parsing/parsing_utils.c\
		parsing/restore_new_args.c\
		parsing/restore_filename.c\
		parsing/redirection_parsing.c\
		execution/action_child.c\
		execution/action_parents.c\
		execution/action_builtin.c\
		execution/execute.c\
		execution/prepare.c\
		execution/heredoc_utils.c\
		execution/exitcode_utils.c\
		execution/utils.c\
		execution/pipe_execution.c\
		execution/execution_node.c\
		error_log/log_file.c\
		expand/expand.c\
		expand/curly_braces.c\
		expand/merge.c\
		expand/quote.c\
		expand/exit_code.c\
		expand/dollar_sign.c\
		expand/utils.c\
		builtin/builtin.c\
		builtin/cd.c\
		builtin/cd_utils.c\
		builtin/echo.c\
		builtin/env_utils.c\
		builtin/env.c\
		builtin/exit.c\
		builtin/export.c\
		builtin/export_utils.c\
		builtin/print_error.c\
		builtin/pwd.c\
		builtin/unset.c\
		utils/print_function.c
MAN_SRCS = $(addprefix $(SRCS_DIR), $(SRCS))
MAN_OBJS = $(MAN_SRCS:.c=.o)

BONUS_SRCS = $(addprefix $(SRCS_DIR), $(SRCS))
BONUS_OBJS = $(MAN_SRCS:.c=.o)

vpath %.c $(SRCS_DIR)

all: $(LIBFT) $(NAME)

%.o: %.c
	@$(CC) $(FLAG) $(COMPILE_FLAG) -c $< -o $@
$(NAME): $(MAN_OBJS)
	@$(CC) $(FLAG) $(COMPILE_FLAG) -o $@ $(MAN_OBJS) $(LIBFT) $(LINK_FLAG) 
	@echo "making minishell"
	@echo "\033[1;33m"
	@echo "                __"
	@echo "     __/~~\\-''- _ |"
	@echo "__- - {            \\"
	@echo "     /             \\"
	@echo "    /        o    o }"
	@echo "    |              ;"
	@echo "                   '"
	@echo "       \\_       (..)"
	@echo "         ''-_ _ _ /"
	@echo "           /"
	@echo "          /"			MINISHELL..
	@echo "\033[0m"
$(LIBFT):
	@make -C ./libft

bonus: $(BONUS_OBJS) $(LIBFT)
	@if [ -f $(NAME) ]; then \
		echo "make: Nothing to be done for 'bonus'."; \
	else \
		$(CC) $(FLAG) $(COMPILE_FLAG) -o $(NAME) $(BONUS_OBJS) $(LIBFT) $(LINK_FLAG); \
		echo "making minishell with bonus"; \
	fi

bonus_re : 
		make fclean
		make bonus
		@echo "rebuilding bonus"

clean:
	@$(RM) $(MAN_OBJS)
	@make clean -C ./libft
	@echo "cleaning"
	@echo "\033[1;33m"
	@echo "                __"
	@echo "     __/~~\\-''- _ |"
	@echo "__- - {            \\"
	@echo "     /             \\"
	@echo "    /        __    _}"
	@echo "    |              ;"
	@echo "                   '"
	@echo "       \\_       (..)"
	@echo "         ''-_ _ _ /"
	@echo "           /"
	@echo "          /" HMM..
	@echo "\033[0m"

fclean: clean
	@$(RM) $(NAME)
	@make fclean -C ./libft
	@echo "fcleaning"

re: fclean all
	@echo "rebuilding"

.PHONY: all re clean fclean bonus bonus_re
