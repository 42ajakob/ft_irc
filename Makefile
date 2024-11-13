# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/12 17:19:16 by apeposhi          #+#    #+#              #
#    Updated: 2024/11/13 18:03:21 by JFikents         ###   ########.fr        #
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
					getChannel.cpp\
					invite.cpp\
					join.cpp\
					kick.cpp\
					mode.cpp\
					part.cpp\
					topic.cpp\
					whoReply.cpp
CHANNEL_SRC		=	$(addprefix channel/, $(_CHANNEL_SRC))

_CLIENT_SRC		=	Client.cpp\
					bufferMethods.cpp\
					clientOpMethods.cpp\
					Operator.cpp\
					registrationMethods.cpp\
					timeoutMethods.cpp
CLIENT_SRC		=	$(addprefix client/, $(_CLIENT_SRC))

_SERVER_SRC		=	Server.cpp\
					handleInvite.cpp\
					handleJoin.cpp\
					handleKick.cpp\
					handleMode.cpp\
					handlePart.cpp\
					handlePrivMsg.cpp\
					handleTopic.cpp\
					handleWho.cpp\
					IO_Loop.cpp\
					receiveMessage.cpp\
					registrationMethods.cpp\
					sendMessage.cpp\
					servOpMethods.cpp\
					timeoutMethods.cpp\
					utils.cpp
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