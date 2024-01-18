# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jrinna <jrinna@student.42lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/02 14:21:09 by jrinna            #+#    #+#              #
#    Updated: 2024/01/18 11:20:01 by jrinna           ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

ifeq ($(DEBUG),)
	DEBUG_FT := 0
else
	DEBUG_FT := 1
endif

ifeq ($(PRINTF),)
	PRINTF_FT := 0
else
	PRINTF_FT := 1
endif

ifeq ($(FREE_DELAY),)
	FREE_DELAY_FT := 0
else
	FREE_DELAY_FT := $(FREE_DELAY)
endif

ifeq ($(POISON_FREE),)
	POISON_FREE_FT := 0
else
	POISON_FREE_FT := 1
endif

ifeq ($(CHECK_FREE),)
	CHECK_FREE_FT := 1
else
	CHECK_FREE_FT := 0
endif

ifeq ($(LOG),)
	LOG_FT := 0
else
	LOG_FT := 1
endif

ifeq ($(MUTEX),)
	MUTEX_FT := 0
else
	MUTEX_FT := 1
endif

ifeq ($(COALESCING),)
	COALESCING_FT := 0
else
	COALESCING_FT := 1
endif

ifeq ($(BEST_FIT),)
	BEST_FIT_FT := 0
else
	BEST_FIT_FT := 1
endif

#update on every project
LST_SRC :=	core/malloc core/free core/realloc core/global core/show_alloc core/calloc \
			\
			memory/memset \
			\
			t_list/t_list \
			\
			utils/free_everything utils/math utils/page_selector utils/pointer_verification utils/ft_printf

NAME := libft_malloc_$(HOSTTYPE).so
LINKNAME := libft_malloc.so

#update to match c or cpp
CC := cc
FILE_EXT := .c
HEADER_EXT := .h
CPPFLAGS := #-std=c++98 #-pedantic

#update if needed
CFLAGS = -Wall -Wextra -Werror -MD -I$(DIR_INC) -g3 -fPIC\
	-DDEBUG_FT=$(DEBUG_FT) \
	-DPRINTF_FT=$(PRINTF_FT) \
	-DFREE_DELAY_FT=$(FREE_DELAY_FT) \
	-DPOISON_FREE_FT=$(POISON_FREE_FT) \
	-DCHECK_FREE_FT=$(CHECK_FREE_FT) \
	-DLOG_FT=$(LOG_FT) \
	-DMUTEX_FT=$(MUTEX_FT) \
	-DCOALESCING_FT=$(COALESCING_FT) \
	-DBEST_FIT_FT=$(BEST_FIT_FT)
DIR_SRC := source#.
SUB_DIR_LST := core utils memory t_list

#shouldn't need to update
RM := rm -rf
MD := mkdir -p

DIR_INC := include
DIR_OBJ := .object

OBJ=$(addprefix $(DIR_OBJ)/,$(addsuffix .o,$(LST_SRC)))
DEP=$(addprefix $(DIR_OBJ)/,$(addsuffix .d,$(LST_SRC)))
SUB_DIR=$(addprefix $(DIR_OBJ)/,$(SUB_DIR_LST))

all : $(NAME)
	@echo "DEBUG_FT IS $(DEBUG_FT)"
	@echo "PRINTF_FT IS $(PRINTF_FT)"
	@echo "FREE_DELAY_FT IS $(FREE_DELAY_FT)"
	@echo "POISON_FREE_FT IS $(POISON_FREE_FT)"
	@echo "CHECK_FREE_FT IS $(CHECK_FREE_FT)"
	@echo "LOG_FT IS $(LOG_FT)"
	@echo "MUTEX_FT IS $(MUTEX_FT)"
	@echo "COALESCING_FT IS $(COALESCING_FT)"
	@echo "BEST_FIT_FT IS $(BEST_FIT_FT)"

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