# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/09 18:19:12 by hskrzypi          #+#    #+#              #
#    Updated: 2025/02/23 17:34:58 by hskrzypi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= philo_bonus

SRC	= main_bonus.c initialization_bonus.c utils_bonus.c monitor_bonus.c eat_think_sleep_bonus.c processes_bonus.c sem_utils_bonus.c cleanup_bonus.c

HEAD	= philo_bonus.h

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
