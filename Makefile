NAME = webserv

SRCS = main.cpp HostsManagement.cpp filemanagement.cpp A_Request.cpp GetRequest.cpp PostRequest.cpp  DeleteRequest.cpp CgiHandler.cpp \
		client.cpp utils/utils.cpp CGI.cpp BoundaryHandler.cpp 

OBJS = ${SRCS:.c=.o}

CC = c++ -fsanitize=address

RM = rm -f

# CFLAGS = -Wall -Wextra -Werror -std=c++98
CFLAGS = -std=c++11

all : $(NAME)

$(NAME) : $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)

clean :
	$(RM) $(NAME)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY : all clean fclean re bonus