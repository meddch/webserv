CC = c++
RED = \033[0;31m
GREEN = \033[0;32m
BLUE = \033[0;34m
NAME = Webserv
RM = rm -f
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 
FILE =  main core/Client  core/Core     core/Server parse/Parse    parse/Utils   request/Request response/Response response/generators

HEADERS = Webserv.hpp core/Client.hpp core/Core.hpp core/Server.hpp parse/Parse.hpp parse/Utils.hpp request/Request.hpp response/Response.hpp 

SRCS = $(addsuffix .cpp, $(FILE))
OBJS = $(addsuffix .o, $(FILE))

%.o : %.cpp $(HEADERS)
	@echo "$(BLUE)Building project ⏳..."
	@$(CC) $(CPPFLAGS)  -c -o $@ $<

all: $(NAME)
	@echo "$(GREEN)✅ ALL DONE ✅"

$(NAME): $(OBJS)
	@$(CC) $(CPPFLAGS) -o $(NAME) $^


clean:
	@$(RM) $(OBJS)
	@echo  "$(RED)OBJS DELETED ☠ "

fclean: clean
	@$(RM) $(NAME) $(LIBFT)
	@echo  "$(RED)ALL DELETED ☠ "


re: fclean all