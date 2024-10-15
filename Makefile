# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/30 12:38:40 by suminkwon         #+#    #+#              #
#    Updated: 2024/10/13 21:39:25 by suminkwon        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

INCLUDE_DIR = ./includes/

RM = rm -rf

LIBFT = ./libft/libft.a

FLAG = -Wall -Wextra -Werror -I $(INCLUDE_DIR) # -fsanitize=address

# COMPILE_FLAG = -I/usr/local/opt/readline/include/

# LINK_FLAG = -lreadline -lncurses -L/usr/local/opt/readline/lib

COMPILE_FLAG = -I/opt/homebrew/opt/readline/include/

LINK_FLAG = -lreadline -lncurses -L/opt/homebrew/opt/readline/lib

# COMPILE_FLAG = -I/usr/include/readline
# LINK_FLAG = -L/usr/local/lib -lreadline

# LINK_FLAG = -lreadline -lncurses -L/
# utils/setting_display.c\

SRCS_DIR = ./srcs/

SRCS =	mini.c \
		utils/setting_display.c\
		utils/ft_strcmp.c\
		utils/ft_strndup.c\
		utils/ft_realloc.c\
		utils/ft_isspace.c\
		utils/free_one.c\
		utils/all_free.c\
		utils/ft_strcpy.c\
		utils/waitpid_status.c\
		tokenize/utils.c\
		tokenize/readline_again.c\
		tokenize/check_set.c\
		tokenize/tokenize.c\
		tokenize/tokenize_operator.c\
		tokenize/operation_error.c\
		tokenize/operation_utils.c\
		tokenize/input_utils.c\
		signal/signal.c \
		redirection/heredoc.c\
		redirection/herestring.c\
		redirection/read_line.c\
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
		execution/execute.c\
		execution/heredoc_utils.c\
		execution/exitcode_utils.c\
		execution/utils.c\
		execution/pipe_execution.c\
		execution/execution_node.c\
		error_log/log_file.c\
		expand/expand.c\
		expand/utils.c\
		builtin/builtin.c\
		builtin/cd.c\
		builtin/echo.c\
		builtin/env_utils.c\
		builtin/env.c\
		builtin/exit.c\
		builtin/export.c\
		builtin/print_error.c\
		builtin/pwd.c\
		builtin/unset.c\
#utils/print_function.c\

MAN_SRCS = $(addprefix $(SRCS_DIR), $(SRCS))
MAN_OBJS = $(MAN_SRCS:.c=.o)

BONUS_SRCS = $(addprefix $(SRCS_DIR), $(SRCS))
BONUS_OBJS = $(MAN_SRCS:.c=.o)

vpath %.c $(SRCS_DIR)
#vpath %.c src라는 라인은 %.c 패턴에 맞는 모든 C 소스 파일을 src 디렉터리에서 찾으라는 의미

all: $(LIBFT) $(NAME)

%.o: %.c
	@$(CC) $(FLAG) $(COMPILE_FLAG) -c $< -o $@
$(NAME): $(MAN_OBJS)
	@$(CC) $(FLAG) $(COMPILE_FLAG) -o $@ $(MAN_OBJS) $(LIBFT) $(LINK_FLAG) 
	@echo "making minishell"

$(LIBFT):
	@make -C ./libft

bonus: $(BONUS_OBJS)
	@$(CC) $(FLAG) $(COMPILE_FLAG) -o $(NAME) $(BONUS_OBJS) $(LIBFT) $(LINK_FLAG) 
	@echo "making minishell with bonus"

bonus re : 
		make fclean
		make bonus
		@echo "rebuilding bonus"

clean:
	@$(RM) $(MAN_OBJS)
	@make clean -C ./libft
	@echo "cleaning"

fclean: clean
	@$(RM) $(NAME)
	@make fclean -C ./libft
	@echo "fcleaning"

re: fclean all
	@echo "rebuilding"

.PHONY: all re clean fclean
