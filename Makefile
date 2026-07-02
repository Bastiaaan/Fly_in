
SRC = src/main.cpp \
	  src/parsing.cpp \
	  src/reference/data.cpp \
	  src/reference/drone.cpp \
	  src/reference/hub.cpp \
	  src/reference/map.cpp \
	  src/reference/system.cpp
	
NAME = fly_in

COMP = c++ -Wall -Werror -Wextra std=c++98
OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(COMP) -o $(NAME)