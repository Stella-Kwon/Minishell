# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/30 12:38:40 by suminkwon         #+#    #+#              #
#    Updated: 2024/10/02 05:20:05 by suminkwon        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

INCLUDE_DIR = ./includes/

RM = rm -rf

LIBFT = ./libft/libft.a

FLAG = -Wall -Wextra -Werror -I $(INCLUDE_DIR)

COMPILE_FLAG = -I/opt/homebrew/opt/readline/include/

LINK_FLAG = -lreadline -lncurses -L/opt/homebrew/opt/readline/lib

SRCS_DIR = ./srcs/

SRCS = minishell.c\
		utils/setting_display.c\
		utils/ft_strcmp.c\
		utils/ft_strndup.c\
		utils/ft_realloc.c\
		utils/ft_isspace.c\
		utils/free_one.c\
		utils/waitpid_status.c\
		tokenize/readline_again.c\
		tokenize/check_set.c\
		tokenize/tokenize.c\
		tokenize/tokenize_operator.c\
		tokenize/store_words.c\
		tokenize/operation_error.c\
		signal/signal.c \
		redirection/heredoc.c\
		redirection/herestring.c\
		redirection/read_line.c\
		parsing/create_parse.c\
		parsing/create_command.c\
		error_log/log_file.c\
		# execution/execution_node.c\

MAN_SRCS = $(addprefix $(SRCS_DIR), $(SRCS))
MAN_OBJS = $(MAN_SRCS:.c=.o)

vpath %.c $(SRCS_DIR)
#vpath %.c src라는 라인은 %.c 패턴에 맞는 모든 C 소스 파일을 src 디렉터리에서 찾으라는 의미
all: $(LIBFT) $(NAME)

%.o: %.c
	@$(CC) $(FLAG) $(COMPILE_FLAG) -c $< -o $@
$(NAME): $(MAN_OBJS)
	@$(CC) $(FLAG) $(COMPILE_FLAG) $(LINK_FLAG) $(LIBFT) -o $@ $(MAN_OBJS)
	@echo "making minishell"

$(LIBFT):
	@make -C ./libft

clean:
	@$(RM) $(MAN_OBJS)
	@make clean -C ./libft
	@echo "cleaning"

fclean: clean
	@$(RM) $(NAME)
	@make fclean -C ./libft
	@echo "fcleaning"

re:
	make fclean
	make all

.PHONY: all re clean fclean
