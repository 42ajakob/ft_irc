# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/12 17:19:16 by apeposhi          #+#    #+#              #
#    Updated: 2024/11/11 18:03:24 by JFikents         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Metadata
AUTHORS			=	apeposhi & jfikents & ajakob
NAME			=	ircserv

# Files
_INCLUDE_FLAGS	=	includes
INCLUDE_FLAGS	=	$(addprefix -I, $(_INCLUDE_FLAGS))

_CHANNEL_SRC	=	Channel.cpp\
					broadcast.cpp\
					channelCreation.cpp\
					invite.cpp\
					Topic.cpp
CHANNEL_SRC		=	$(addprefix channel/, $(_CHANNEL_SRC))

_CLIENT_SRC		=	Client.cpp\
					bufferMethods.cpp\
					clientOpMethods.cpp\
					Operator.cpp\
					registrationMethods.cpp\
					timeoutMethods.cpp
CLIENT_SRC		=	$(addprefix client/, $(_CLIENT_SRC))

_SERVER_SRC		=	Server.cpp\
					Invite.cpp\
					joinChannel.cpp\
					mainLoop.cpp\
					Oper.cpp\
					parsing.cpp\
					PrivMsg.cpp\
					receiveMessage.cpp\
					sendMessage.cpp\
					serverStaticMethods.cpp\
					timeouts.cpp\
					Topic.cpp\
					utils.cpp\
					registrationMethods.cpp
SERVER_SRC		=	$(addprefix server/, $(_SERVER_SRC))

_SRC			=	main.cpp $(CLIENT_SRC) $(SERVER_SRC) $(CHANNEL_SRC)
SRC				=	$(addprefix src/, $(_SRC))
OBJ				=	$(SRC:src/%.cpp=bin/%.o)

# Compiler and Flags
CXX				:=	c++
CXXFLAGS		:=	-Wall -Wextra -Werror -std=c++17 -MMD -MP

ifdef DEBUG
CXXFLAGS		+=	-g3
endif

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