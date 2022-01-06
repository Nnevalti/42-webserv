# This is a minimal set of ANSI/VT100 color codes
Color_Off=\033[0m
_GREEN=\033[1;32m
_CYAN=\033[1;36m
BRed=\033[1;31m"         # Red
_Orange=\033[1;96m        # Blue
_Purple=\033[1;35m

# DIRECTORIES

BUILD	= .build
SRC_DIR	= ./
INC		= -I./

# FILES
NAME	= autoindex

SRC		= autoindex.cpp

DEP             := $(SRC:%.cpp=$(BUILD)/%.d)
OBJ             := $(SRC:%.cpp=$(BUILD)/%.o)

# COMPILATION
CC		= clang++
CFLAGS	= -Wall -Werror -Wextra
DFLAGS  = -MP -MMD

$(NAME): $(OBJ)
	@$(CC) $(OBJ) -o $(NAME)
	@echo "${_GREEN}The executable file of autoindex Has been created!${Color_Off}"


all: $(NAME)

clean:
	@rm -rf $(BUILD)
	@echo "\033[1;34mExecute:\t\033[1;33mCleaning build\t\033[0;32m[OK]\033[0m"

fclean: clean
	@rm -f $(NAME)
	@echo "\033[1;34mExecute:\t\033[1;33mCleaning exec\t\033[0;32m[OK]\033[0m"

re: fclean all

$(BUILD):
	@echo "\033[1;32m\nCreation of .build\033[0m \n"
	@mkdir $@

$(BUILD)/%.o: $(SRC_DIR)%.cpp | $(BUILD)
	@printf "\033[1;34mExecute: \t\033[1;33mCompilation of \033[1;36m$(notdir $<) \033[0m \
	%-33.33s\r"
	@$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@ $(INC)

-include $(DEP)

.PHONY: all clean fclean re
