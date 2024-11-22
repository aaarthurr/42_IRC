NAME = cub3d

CC = cc

CFLAGS += -g -Wall -Wextra -I/usr/include -Imlx_linux -IInclude  -Imlx

HEADER = cube3d.h

LIB = -Lmlx_linux -lmlx_Linux -lXext -lX11 -lm  -IO3 -lz -L/usr/lib

ERROR = check_error check_error_tools check_error_tools2

GNL = get_next_line gnl_utils

MAIN = main render render2 render3 render4 render5 input input2

INITIALIZATION = parse_map init_data init_mlx_data init_map_data init_trace_data init_bonus_data init_bonus_data2 parse_map_tools parse_map_tools2

TOOLS = exit_manager tools debug gnl_tools render_tools ft_strtrim

BONUSES = minimap1 minimap2 minimap3 minimap4 doors collectibles ennemies sound cinematic1 cinematic2 cinematic3 cinematic4

SRC = $(addsuffix .c, $(addprefix src/GNL/, $(GNL)))\
		$(addsuffix .c, $(addprefix src/Initialization/, $(INITIALIZATION)))\
		$(addsuffix .c, $(addprefix src/Tools/, $(TOOLS)))\
		$(addsuffix .c, $(addprefix src/Main/, $(MAIN)))\
		$(addsuffix .c, $(addprefix src/Error/, $(ERROR)))\
		$(addsuffix .c, $(addprefix src/Bonuses/, $(BONUSES)))\

OBJ = $(SRC:c=o)

all: $(NAME)

$(NAME): $(OBJ)
	@echo "\033[0;32mCube3d Compiled !"
	@$(CC) $(CFLAGS)  $(OBJ) $(LIB) -o $(NAME)
	@echo "\n\033[0mCube3d to execute the program !"

%.o: %.c
	@printf "\033[0;33mGenerating Cube3d objects... %-33.33s\r\n" $@
	@${CC} ${CFLAGS} -c $< -o $@

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