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
CXX = c++

# Flags de compilation
CXXFLAGS = -Wall -Werror -Wextra -std=c++98

# Fichiers sources du programme
SOURCES =   main.cpp\
			serveur.cpp\
			client.cpp\
			User_cmd.cpp \
			channel_cmd.cpp \
			channel.cpp \
			Join_cmd.cpp \
			utlis_function.cpp\
			Nick_cmd.cpp \
			Topic_cmd.cpp \
			Mode_cmd.cpp \
			cmd_lil.cpp \
			cmd_prv_msg.cpp \

# Fichiers objets

all: $(NAME)

$(NAME):
	$(CXX) $(CXXFLAGS) $(SOURCES)-o $(NAME)

clean:
	

fclean: clean
	rm -f $(NAME)

re : fclean all
