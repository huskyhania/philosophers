# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/09 18:19:12 by hskrzypi          #+#    #+#              #
#    Updated: 2025/02/01 17:16:47 by hskrzypi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= philo

SRC	= main.c initialization.c utils.c monitor.c routine.c eat_think_sleep.c threads.c

HEAD	= philo.h

OBJS	= ${SRC:.c=.o}

RM		= rm -f

CC		= cc

CFLAGS	= -Wall -Wextra -Werror

all: ${NAME}

%.o: %.c ${HEAD}
	${CC} ${CFLAGS} -c $< -o $@

${NAME}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re
