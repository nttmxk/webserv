NAME		=	webserv
CXX			=	c++
CXXFLAGS	=	-std=c++98 -pedantic -g3 -fsanitize=address
SRCS		=	main.cpp \
				webserv3/HttpServer.cpp \
				webserv3/connection/Connection.cpp \
				webserv3/response/Response.cpp \
				webserv3/connection/Multiplex.cpp \
				configParser/BaseServer.cpp \
				configParser/Config.cpp \
				parseRequest/Request.cpp


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