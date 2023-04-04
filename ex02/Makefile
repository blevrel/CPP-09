NAME = PmergeMe

SRCS =	main.cpp	\
		PmergeMe.cpp

OBJS = ${SRCS:.cpp=.o}

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 -g

RM = rm -f

all: ${NAME}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

.cpp.o:
	${CC} ${CFLAGS} -c $< -o ${<:.cpp=.o}

${NAME}:${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}

.PHONY: all clean fclean re
