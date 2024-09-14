# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/12 17:19:16 by apeposhi          #+#    #+#              #
#    Updated: 2024/09/12 17:26:15 by apeposhi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Metadata
AUTHORS = apeposhi & jfikents
NAME = ircserv
// WIP
SERVER = src/server
CLIENT = src/client

# Directories
SRC_DIR = src
OBJ_DIR = obj

# Files
SRC = main.cpp client/Client.cpp server/Server.cpp utils/Utils.cpp
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))

# Compiler and Flags
CXX       := c++
CXXFLAGS  := -Wall -Wextra -Werror -std=c++17
EXE_FLAG  := -o
EXEC_C      := $(CLIENT)
EXEC_S	  := $(SERVER)

# Suffix Rules
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Targets
all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(EXE_FLAG) $(EXEC_C) $(EXEC_S)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(EXEC_S) $(EXEC_C)

re:	fclean all

# Phony Targets
.PHONY:	all clean fclean re
