.PHONY:		fclean, all, clear, clean, re

NAME					=	bot

CMP						=	c++
FLG						=	-Wall -Wextra -Werror -std=c++98 -fsanitize=address
RM						=	rm -rf
BUILD_DIR				= 	build/

INC_DIR					=	includes
INC_FILE				= 	bot
INC						= 	$(addsuffix .hpp, $(addprefix $(INC_DIR)/, $(INC_FILE)))

SRCS_DIR				=	srcs
SRCS_FILE				=	main \
							bot
SRCS					=  	$(addsuffix .cpp, $(addprefix $(SRCS_DIR)/, $(SRCS_FILE)))

OBJS			    	= 	$(SRCS:%.cpp=$(BUILD_DIR)%.o)

$(BUILD_DIR)%.o:		%.cpp $(INC)
						@echo -n .
						@mkdir -p $(@D)
						@$(CMP) $(FLG) -I$(INC_DIR) -c $< -o $@

all:		$(NAME)

clear:
			clear

$(NAME):	$(INC) $(OBJS) $(SRCS)
			@$(CMP) $(FLG) $(OBJS) -I$(INC_DIR) -o $(NAME)
			@echo "\033[32m\n- $(NAME) executable ready.\033[0m"

clean:
			@$(RM) $(BUILD_DIR)
			@printf "$(YELLOW)    - Removed objs.o.$(RESET)\n"

fclean:
			@$(RM) $(NAME)
			@$(RM) $(BUILD_DIR)
			@printf "$(RED)    - Executable removed.$(RESET)\n"

re:			fclean all

#Colors:
GREEN		=	\e[92;5;118m
YELLOW		=	\e[93;5;226m
GRAY		=	\e[33;2;37m
RESET		=	\e[0m
CURSIVE		=	\e[33;3m
RED = \033[0;31m