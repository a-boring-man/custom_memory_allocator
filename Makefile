# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jrinna <jrinna@student.42lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/02 14:21:09 by jrinna            #+#    #+#              #
#    Updated: 2023/11/14 11:23:03 by jrinna           ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

#update on every project
LST_SRC :=	malloc free realloc page_selector create_page glue_page_together

NAME := libft_malloc_$(HOSTTYPE).so
LINKNAME := libft_malloc.so

#update to match c or cpp
CC := gcc
FILE_EXT := .c
HEADER_EXT := .h
CPPFLAGS := #-std=c++98 #-pedantic

#update if needed
CFLAGS = -Wall -Wextra -Werror -MD -O2 -I$(DIR_INC) -g3#-fsanitize=address
DIR_SRC := source#.
SUB_DIR_LST := .

#shouldn't need to update
RM := rm -rf
MD := mkdir -p

DIR_INC := include
DIR_OBJ := .object

OBJ=$(addprefix $(DIR_OBJ)/,$(addsuffix .o,$(LST_SRC)))
DEP=$(addprefix $(DIR_OBJ)/,$(addsuffix .d,$(LST_SRC)))
SUB_DIR=$(addprefix $(DIR_OBJ)/,$(SUB_DIR_LST))

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) -shared $^ -o $@
	$(RM) $(LINKNAME)
	ln -s $(NAME) $(LINKNAME)

$(DIR_OBJ)/%.o : $(DIR_SRC)/%$(FILE_EXT) Makefile | $(SUB_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

$(SUB_DIR) :
	$(MD) $@

clean :
	$(RM) $(DIR_OBJ)

fclean : clean
	$(RM) $(NAME) $(LINKNAME)

re : fclean all

.PHONY : all clean fclean re

-include $(DEP)