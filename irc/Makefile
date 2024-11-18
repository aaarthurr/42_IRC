NAME2 = ircserv

NAME1 = irclient

CC = c++ -std=c++98

CFLAGS = -Wall -Wextra -Werror

SRC1 = main.cpp 

SRC2 = serveur.cpp

OBJ1 = $(SRC1:cpp=o)

OBJ2 = $(SRC2:cpp=o)

all: $(NAME1) $(NAME2)

$(NAME1): $(OBJ1)
	@echo "\033[38;5;213mircCompiled"
	@$(CC) $(CFLAGS) -o $(NAME1) $(OBJ1)

$(NAME2): $(OBJ2)
	@$(CC) $(CFLAGS) -o $(NAME2) $(OBJ2)
	@echo "\n\033[1;31m./irc to execute program\n"


%.o: %.cpp
	@printf "\033[38;5;213mGenerating irc objects... %-33.33s\r\n" $@
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "\n\033[1;31mRemoving binaries..."
	@rm -f $(OBJ1) $(OBJ2)
	@echo "\033[0m"

fclean: clean
	@echo "\n\033[1;31mDeleting executable..."
	@rm -f $(NAME1) $(NAME2)
	@echo "\033[0m"

re: fclean all


.PHONY: clean fclean re 