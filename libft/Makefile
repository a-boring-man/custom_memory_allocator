# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jrinna <jrinna@student.42lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/09/20 11:14:14 by jrinna            #+#    #+#              #
#    Updated: 2021/11/15 13:17:02 by jrinna           ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

SRCS = ft_putnbr_fd.c ft_putendl_fd.c ft_putstr_fd.c ft_putchar_fd.c ft_striteri.c ft_strmapi.c ft_itoa.c ft_split.c ft_strtrim.c ft_strjoin.c ft_substr.c ft_strdup.c ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c

OBJ = $(SRCS:.c=.o)

BONUS = ft_lstmap.c ft_lstiter.c ft_lstclear.c ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c

OBJ_BONUS = $(BONUS:.c=.o)

NAME = libft.a

CC = gcc

CFLAGS = -Wall -Wextra -Werror -I.

${NAME}: ${OBJ}
	ar rcs ${NAME} ${OBJ}

%.o : %.c libft.h Makefile
	$(CC) $(CFLAG) -c -o $@ $<

all : ${NAME}

clean :
	$(RM) ${OBJ} ${OBJ_BONUS}

fclean : clean
	$(RM) $(NAME)

re :
	$(MAKE) fclean
	$(MAKE) all

bonus : ${OBJ} ${OBJ_BONUS}
	ar rcs ${NAME} ${OBJ} ${OBJ_BONUS}
