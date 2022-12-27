NAME		=	webserv
CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 -pedantic
SRCS		=	main.cpp HttpServer.cpp Connection.cpp Response.cpp
OBJS		=	$(SRCS:.cpp=.o)

all	:	$(NAME)

$(NAME)	: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)
	@printf "=> $(NAME) created.\n"


%.o	:	%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean	:
	@rm -rf $(OBJS)
	@printf "=> $(NAME) object files removed.\n"

fclean	:	clean
	@rm -rf $(NAME)
	@printf "=> $(NAME) removed.\n"

re	:	fclean all

.PHONY : all clean fclean re
