# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fboughan <fboughan@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/12 12:40:22 by fboughan          #+#    #+#              #
#    Updated: 2024/03/18 11:41:42 by fboughan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
OUT_DIR = ./obj
SRC = $(addprefix src/, main.c \
						utils/atoi.c \
						utils/get_time.c \
						utils/sleep.c \
						utils/destroy.c \
						utils/is_valid.c \
						init/init_philo.c \
						init/init_args.c \
						init/init_forks.c \
						philo_check/eat_dead.c \
						thread/join_thread.c \
						thread/thread_logique.c \
						thread/monitoring.c \
						thread/supervise.c \
)

OBJS := $(SRC:src/%.c=$(OUT_DIR)/%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -Iincludes 
# LFT = libft/libft.a

# Specify Bash as the shell for Makefile
SHELL = /bin/bash

# ANSI escape sequences for green color
GREEN = \033[0;32m
NC = \033[0m

.PHONY: all clean fclean re 

all:  $(NAME)

# libft:
# 	@make -C libft/

$(NAME): $(OBJS) 
	@$(CC) $(CFLAGS) $(OBJS)  -lreadline -o $(NAME)

$(OUT_DIR)/%.o: src/%.c
	@mkdir -p $(OUT_DIR)
	@$(CC) $(CFLAGS)  -c $< -o $@
	@touch .compiled_files.tmp
	@COMPILED_FILES=$$(cat .compiled_files.tmp); \
	TOTAL_FILES=$(words $(SRC)); \
	COMPILED_FILES=$$(expr $$COMPILED_FILES + 1); \
	echo $$COMPILED_FILES > .compiled_files.tmp; \
	printf "Compiling: [ \033[32m"; \
	for ((i = 0; i < $$COMPILED_FILES; i++)); do \
	    printf "█"; \
	done; \
	for ((i = $$COMPILED_FILES; i < $$TOTAL_FILES; i++)); do \
	    printf " "; \
	done; \
	printf "\033[0m] ($$COMPILED_FILES/$$TOTAL_FILES files compiled)\r"; \
	
clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)
	@rm -f .compiled_files.tmp

re: fclean all