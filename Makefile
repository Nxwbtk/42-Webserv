# ***** EXECUTABLE ***** #
NAME = webserv
# ***** COLORS ********* #
RED		= \033[0;31m
GREEN	= \033[0;32m
YELLOW	= \033[0;33m
BLUE	= \033[0;34m
MAGENTA	= \033[0;35m
CYAN	= \033[0;36m
BOLD	= \033[1m
ITALIC	= \033[3m
ULINE	= \033[4m
BLINK	= \033[5m
NC		= \033[0m

# ***** COMPILATION **** #
CXX = c++ -g3
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
INC = -I./inc
RM = rm -rf

# ***** HEADERS ******** #
HDR = ./inc/Webserv.hpp

# ***** SOURCES ******** #
SRC_DIR = ./src/
SRC = main.cpp \
		Webserv.cpp \
		ConfigParser.cpp \
		Helpers.cpp \
		HttpHandler.cpp \
		Webserv.cpp \

SRCS = $(addprefix $(SRC_DIR), $(SRC))

# ***** OBJECTS ******** #
OBJ_DIR = ./obj/
OBJ = $(SRC:.cpp=.o)
OBJS = $(addprefix $(OBJ_DIR), $(OBJ))

# ***** RULES ********** #
all: $(OBJ_DIR) $(NAME)

 $(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(HDR)
	@echo "$(MAGENTA) $(CXX) $(CXXFLAGS) -c $< -o $@ $(NC)"
	@$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

$(NAME): $(OBJS)
	@echo "\n$(BLUE) $(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(NC)"
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $@
	@echo "\n$(GREEN)---- (っ▀¯▀)つ $(NAME) is ready! ----$(NC)\n"

clean:
	@$(RM) $(OBJ_DIR)
	@echo "\n$(YELLOW) $(RM) $(OBJ_DIR) $(NC)"
	@echo "\n$(RED)---- ᕦ(ò_óˇ)ᕤ $(NAME) is clean! ----$(NC)\n"

fclean: clean
	@$(RM) $(NAME)
	@echo "\n$(YELLOW) $(RM) $(NAME) $(NC)"
	@echo "\n$(RED)---- (╯°□°）╯︵ ┻━┻ $(NAME) is gone! ----$(NC)\n"

re: fclean all

# ***** MEMORY LEAKS **** #
mem1:
	valgrind --leak-check=summary ./$(NAME)
mem2:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)
mem3:
	valgrind -s ./$(NAME)

.PHONY : all clean fclean re mem1 mem2 mem3
