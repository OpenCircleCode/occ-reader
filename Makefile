# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thibautpierron <thibautpierron@student.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/07/21 11:32:23 by tpierron          #+#    #+#              #
#    Updated: 2017/11/17 21:39:54 by thibautpier      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re

NAME = openCircleCode

CC = clang++
SRC =	src/Reader.cpp \
		main.cpp


FLAGS = -lopencv_highgui -lopencv_core -lopencv_imgproc
OBJ_PATH = ./obj/
OBJ_NAME = $(SRC:.cpp=.o)

PATHS = -I/usr/local/Cellar/opencv@2/2.4.13.4/include/
LIBS = -L/usr/local/Cellar/opencv@2/2.4.13.4/lib

OBJ = $(addprefix $(OBJ_PATH), $(OBJ_NAME))

all: $(NAME)
$(OBJ_PATH)%.o: %.cpp
	@mkdir $(OBJ_PATH) 2> /dev/null || echo "" > /dev/null
	@mkdir $(OBJ_PATH)/src 2> /dev/null || echo "" > /dev/null
	$(CC) -o $@ -c $(PATHS) $< -std=c++11

$(NAME): $(OBJ)
	$(CC) $(LIBS) $(FLAGS) -o $@ $^

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all
