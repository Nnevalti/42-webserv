# This is a minimal set of ANSI/VT100 color codes
Color_Off=\033[0m
_GREEN=\033[1;32m
_CYAN=\033[1;36m
BRed=\033[1;31m         # Red
_Orange=\033[1;96m        # Blue
_Purple=\033[1;35m

# DIRECTORIES

BUILD	= .build

SRC_DIR	= ./src/
DIR		= parser webserv config client t_network request response autoindex cgi
DIRS	= $(addprefix $(BUILD)/, $(DIR))
CLASSINC= $(addprefix -I $(SRC_DIR), $(DIR))

INC		= -I./include

# FILES
NAME	= webserv

MAIN	= main.cpp

PARSER	= Parser.cpp ParseConfig.cpp ParseRequest.cpp ParseResponse.cpp

WEBSERV	= Webserv.cpp

CLIENT	= Client.cpp

CONFIG	= Config.cpp

REQUEST	= Request.cpp

NET		= t_network.cpp

RESPONSE= Response.cpp ConfigResponse.cpp

AUTOINDEX= autoindex.cpp

CGI		= Cgi.cpp

SRC		= $(MAIN) \
		  $(addprefix parser/, $(PARSER)) \
		  $(addprefix webserv/, $(WEBSERV)) \
		  $(addprefix config/, $(CONFIG)) \
		  $(addprefix client/, $(CLIENT)) \
		  $(addprefix t_network/,$(NET)) \
		  $(addprefix request/, $(REQUEST)) \
		  $(addprefix response/, $(RESPONSE)) \
		  $(addprefix autoindex/, $(AUTOINDEX)) \
		  $(addprefix cgi/, $(CGI))


DEP		:= $(SRC:%.cpp=$(BUILD)/%.d)
OBJ		:= $(SRC:%.cpp=$(BUILD)/%.o)

# COMPILATION
CC		= clang++
CFLAGS	= -Wall -Werror -Wextra -std=c++98
DFLAGS  = -MP -MMD

$(NAME): $(OBJ)
	@$(CC) $(OBJ) -o $(NAME)
	@echo ""
	@echo "${_GREEN}The executable file of ${Color_Off}${_CYAN}"
	@echo "██╗    ██╗███████╗██████╗ ███████╗███████╗██████╗ ██╗   ██╗"
	@echo "██║    ██║██╔════╝██╔══██╗██╔════╝██╔════╝██╔══██╗██║   ██║"
	@echo "██║ █╗ ██║█████╗  ██████╔╝███████╗█████╗  ██████╔╝██║   ██║"
	@echo "██║███╗██║██╔══╝  ██╔══██╗╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝"
	@echo "╚███╔███╔╝███████╗██████╔╝███████║███████╗██║  ██║ ╚████╔╝ "
	@echo " ╚══╝╚══╝ ╚══════╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝ ${Color_Off}"
	@echo "                                          ${_GREEN}Has been created!${Color_Off}"


all: $(NAME)

clean:
	@rm -rf $(BUILD)
	@rm -rf test/root
	@echo "\033[1;34mExecute:\t\033[1;33mCleaning build\t\033[0;32m[OK]\033[0m"

fclean: clean
	@rm -f $(NAME)
	@echo "\033[1;34mExecute:\t\033[1;33mCleaning exec\t\033[0;32m[OK]\033[0m"

re: fclean all

$(BUILD):
	@echo "\033[1;32m\nCreation of .build\033[0m \n"
	@mkdir $@ $(DIRS)

$(BUILD)/%.o: $(SRC_DIR)%.cpp | $(BUILD)
	@printf "\033[1;34mExecute: \t\033[1;33mCompilation of \033[1;36m$(notdir $<) \033[0m \
	%-33.33s\r"
	@$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@ $(INC) $(CLASSINC)


-include $(DEP)

.PHONY: all clean fclean re
