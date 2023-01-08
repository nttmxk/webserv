# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jinoh <jinoh@student.42seoul.kr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/24 01:39:25 by jinoh             #+#    #+#              #
#    Updated: 2022/09/24 01:39:26 by jinoh            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = parse
CC = c++
#CFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS =	main.cpp \
		Request.cpp \
		Uri.cpp

OBJS = $(SRCS:%.cpp=%.o)

%.o: %.cpp
		$(CC) $(CFLAGS) -c $< -o $(<:.cpp=.o)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

all : $(NAME)

clean:
		rm -rf $(OBJS)

fclean: clean
		rm -rf $(NAME)

re:	fclean all

.PHONY: all clean fclean re
