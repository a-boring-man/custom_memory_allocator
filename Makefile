# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jrinna <jrinna@student.42lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/02 14:21:09 by jrinna            #+#    #+#              #
#    Updated: 2023/12/14 09:35:29 by jrinna           ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

#update on every project
LST_SRC :=	core/malloc core/free core/realloc core/grimoire core/show_alloc \
			\
			bonus_features/poisoning bonus_features/find_free_block_bonus_algo \
			bonus_features/coalescing bonus_features/mutex \
			\
			memory/create_page memory/format_new_page memory/mark_allocated \
			memory/find_free_block memory/mark_free \
			\
			t_list/add_block_to_t_list t_list/remove_block_from_t_list t_list/apply_to_all_element \
			\
			utils/ceilling utils/page_size utils/page_selector utils/pointer_verification \
			\
			ft_printf

NAME := libft_malloc_$(HOSTTYPE).so
LINKNAME := libft_malloc.so

#update to match c or cpp
CC := cc
FILE_EXT := .c
HEADER_EXT := .h
CPPFLAGS := #-std=c++98 #-pedantic

#update if needed
CFLAGS = -Wall -Wextra -Werror -MD -I$(DIR_INC) -g3 -fPIC#-fsanitize=address
DIR_SRC := source#.
SUB_DIR_LST := core utils bonus_features memory t_list

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
	$(CC) $(CFLAGS) $(CPPFLAGS) -DRED_ZONE_DEBUG=1 -DFREE_DELAY=0 -o $@ -c $<

$(SUB_DIR) :
	$(MD) $@

clean :
	$(RM) $(DIR_OBJ)

fclean : clean
	$(RM) $(NAME) $(LINKNAME)

re : fclean all

.PHONY : all clean fclean re

-include $(DEP)