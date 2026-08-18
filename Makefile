
NAME := fly_in

SRC_DIR := src/
SRC_REF := $(SRC_DIR)reference/

SRCS := $(SRC_DIR)main.cpp \
	  $(SRC_DIR)fly_in.cpp \
	  $(SRC_DIR)utils.cpp \
	  $(SRC_REF)algorithm.cpp \
	  $(SRC_REF)connection.cpp \
	  $(SRC_REF)drone.cpp \
	  $(SRC_REF)hub.cpp \
	  $(SRC_REF)map.cpp \
	  $(SRC_REF)system.cpp \
	  $(SRC_REF)validator.cpp \
	  $(SRC_REF)visuals.cpp

# Color declarations below;

RESET := \033[0m
INFO := \033[36m
SUCCESS := \033[32m
ERROR := \033[31m


OBJ_DEST := obj/
OBJ = $(addprefix $(OBJ_DEST),$(notdir $(SRCS:.cpp=.o)))

CC := c++
CFLAGS := -Wall -Werror -Wextra -std=c++23 -Ireference -Iraylib/include
LDFLAGS := -Lraylib/lib -lraylib -Wl,-rpath,./raylib/lib

vpath %.cpp src src/reference

.PHONY: all $(NAME) re clean fclean

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(OBJ) $(LDFLAGS) -o $(NAME)

$(OBJ_DEST)%.o: %.cpp
	@mkdir -p $(OBJ_DEST)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(INFO)Compiling: $<$(RESET)"

re: fclean all

clean:
	@rm -rf $(OBJ_DEST)
	@echo "cleaning obj/ folder"

fclean: clean
	@rm -f $(NAME)
	@echo "full cleaning obj/ folder AND fly-in"