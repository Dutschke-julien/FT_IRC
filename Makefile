# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/24 11:12:17 by jdutschk          #+#    #+#              #
#    Updated: 2023/07/24 11:15:17 by jdutschk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Makefile pour compiler le programme ircserv en C++

# Nom de l'exécutable
NAME = ircserv

# Compilateur
CXX = g++

# Flags de compilation
CXXFLAGS = -Wall -Werror -Wextra -std=c++98

# Fichiers sources du programme
SOURCES = main.cpp

# Fichiers objets
OBJECTS = $(SOURCES:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(NAME)

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)
