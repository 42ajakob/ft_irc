AUTHORS = apeposhi & jfikents

# Compiler

CC = g++

# Flags
FLAGS = -Wall -Wextra -Werror -std=c++17

# Directories
SRC_DIR = src
OBJ_DIR = obj

# Files
SRC = main.cpp client/Client.cpp server/Server.cpp utils/Utils.cpp
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))