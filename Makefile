NAME := fly_in
DEBUG_NAME := fly_in_debug

SRC_DIR := src/
SRC_REF := $(SRC_DIR)reference/
PARS_REF := parser

SRCS := $(SRC_DIR)main.cpp \
      $(SRC_DIR)fly_in.cpp \
      $(SRC_DIR)utils.cpp \
      $(SRC_REF)algorithm.cpp \
      $(SRC_REF)connection.cpp \
      $(SRC_REF)drone.cpp \
      $(SRC_REF)hub.cpp \
      $(SRC_REF)map.cpp \
      $(SRC_REF)system.cpp \
      $(SRC_REF)visuals.cpp

# Color declarations below;

RESET := \033[0m
DEBUG := \033[35m
INFO := \033[36m
SUCCESS := \033[32m
ERROR := \033[31m

OBJ_DEST := obj/
OBJ = $(addprefix $(OBJ_DEST),$(notdir $(SRCS:.cpp=.o)))
DEBUG_OBJ = $(addprefix $(OBJ_DEST),$(notdir $(SRCS:.cpp=.debug.o)))

CC := c++
CC_DEBUG := c++
CFLAGS := -Wall -Werror -Wextra -std=c++20 -Ireference -Iraylib/include -Iinclude
CFLAGS_DEBUG := $(CFLAGS) -g -O0 -DDEBUG
LDFLAGS := -Lraylib/lib -lraylib -Wl,-rpath,./raylib/lib

vpath %.cpp src src/reference

.PHONY: all install parser-install $(NAME) run re clean fclean debug lint

all: install

install: parser-install $(NAME)

parser-install:
	+$(MAKE) -C $(PARS_REF) install

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(NAME)

debug: $(DEBUG_NAME)

$(DEBUG_NAME): $(DEBUG_OBJ)
	$(CC) $(DEBUG_OBJ) $(LDFLAGS) -o $(DEBUG_NAME)

$(OBJ_DEST)%.o: %.cpp
	@mkdir -p $(OBJ_DEST)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(INFO)Compiling: $<$(RESET)"

$(OBJ_DEST)%.debug.o: %.cpp
	@mkdir -p $(OBJ_DEST)
	@$(CC) $(CFLAGS_DEBUG) -c $< -o $@
	@echo "$(DEBUG)Compiling debug: $<$(RESET)"

run:
	@rm -rf $(PARS_REF)/result/*
	+$(MAKE) -C $(PARS_REF) run
	./$(NAME)

re: fclean all

lint:
	+$(MAKE) -C $(PARS_REF) lint

clean:
	@rm -rf $(OBJ_DEST) $(DEBUG_OBJ)
	+$(MAKE) -C $(PARS_REF) clean
	@echo "cleaning obj/ folder and removing __pycache__"

fclean: clean
	@rm -f $(NAME) $(DEBUG_NAME)
	+$(MAKE) -C $(PARS_REF) full_clean
	@echo "full cleaning obj/ folder AND fly-in binaries AND removing .venv and __pycach__"