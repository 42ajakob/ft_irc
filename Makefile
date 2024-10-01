# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/12 17:19:16 by apeposhi          #+#    #+#              #
#    Updated: 2024/10/01 15:59:30 by JFikents         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Metadata
AUTHORS		= apeposhi & jfikents & ajakob
NAME		= ircserv

# Files
_INCLUDE_FLAGS	= includes
INCLUDE_FLAGS	= $(addprefix -I, $(_INCLUDE_FLAGS))

_CLIENT_SRC	= Client.cpp
CLIENT_SRC	= $(addprefix client/, $(_CLIENT_SRC))

_SERVER_SRC	= Server.cpp
SERVER_SRC	= $(addprefix server/, $(_SERVER_SRC))

_UTILS_SRC	= Utils.cpp
UTILS_SRC	= $(addprefix utils/, $(_UTILS_SRC))

_SRC		= main.cpp $(CLIENT_SRC) $(SERVER_SRC) $(UTILS_SRC)
SRC			= $(addprefix src/, $(_SRC))
OBJ			= $(SRC:src/%.cpp=bin/%.o)

# Compiler and Flags
CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++17 -MMD -MP

# Targets
all: $(NAME)

bin:
	@mkdir -p $@

bin/%.o: src/%.cpp | bin
	$(CXX) $(CXXFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re:	fclean $(NAME)

# Phony Targets
.PHONY:	all clean fclean re

-include $(OBJ:.o=.d)