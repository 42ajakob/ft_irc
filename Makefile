# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/12 17:19:16 by apeposhi          #+#    #+#              #
#    Updated: 2024/10/17 16:56:13 by JFikents         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Metadata
AUTHORS		= apeposhi & jfikents & ajakob
NAME		= ircserv

# Files
_INCLUDE_FLAGS	= includes
INCLUDE_FLAGS	= $(addprefix -I, $(_INCLUDE_FLAGS))

_CHANNEL_SRC	= Channel.cpp
CHANNEL_SRC		= $(addprefix channel/, $(_CHANNEL_SRC))

_CLIENT_SRC	= Client.cpp
CLIENT_SRC	= $(addprefix client/, $(_CLIENT_SRC))

_SERVER_SRC	=	Server.cpp\
				serverStaticMethods.cpp\
				mainLoop.cpp
SERVER_SRC	= $(addprefix server/, $(_SERVER_SRC))

_UTILS_SRC	= Utils.cpp
UTILS_SRC	= $(addprefix utils/, $(_UTILS_SRC))

_SRC		= main.cpp $(CLIENT_SRC) $(SERVER_SRC) $(UTILS_SRC) $(CHANNEL_SRC)
SRC			= $(addprefix src/, $(_SRC))
OBJ			= $(SRC:src/%.cpp=bin/%.o)

# Compiler and Flags
CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++17 -MMD -MP

# Targets
all: $(NAME)

bin:
	@mkdir -p bin/client bin/server bin/utils bin/commands bin/channel

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