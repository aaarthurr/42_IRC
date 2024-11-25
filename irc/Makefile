NAME = irc

CC = g++

CFLAGS += -g -Wall -Wextra -Werror -std=c++98

Server = Server User main HandleRequest

SRC = $(addsuffix .cpp,  $(Server))\

OBJ = $(SRC:cpp=o)

all: $(NAME)

$(NAME): $(OBJ)
	@echo "\033[0;32mIrc Compiled !"
	@$(CC) $(CFLAGS)  $(OBJ) -o $(NAME)
	@echo "\n\033[0mIrc to execute the program !"

%.o: %.cpp
	@printf "\033[0;33mGenerating Irc objects... %-33.33s\r\n" $@
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "\nRemoving binaries..."
	@rm -f $(OBJ)
	@echo "\033[0m"

fclean:
	@echo "Deleting objects..."
	@rm -f $(OBJ)
	@echo "\nDeleting executable..."
	@rm -f $(NAME)
	@echo "\033[0m"

re: fclean all


.PHONY: clean fclean re 