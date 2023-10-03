/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serveur.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 15:19:56 by jdutschk          #+#    #+#             */
/*   Updated: 2023/10/03 17:01:35 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVEUR_HPP
# define SERVEUR_HPP

# include<iostream>
# include<iterator>
# include <stdio.h>
# include <iostream>
# include <stdlib.h>
# include <string>
# include <map>
# include <vector>
# include <unistd.h>
# include <cstring>
# include <sstream>
# include <arpa/inet.h>
# include <netinet/in.h>
# include "client.hpp"
# include "channel.hpp"
# define MAX_CLIENTS 10



bool isNickTaken(const std::vector<std::string>& nameVector, const std::string& newNick);
void del_Nick_toks(std::vector<std::string>& nameVector, std::string& Nick);

class Serveur
{
	private:
	
	std::map <int, Client> _mapClients;
	std::vector<int> _list_Clients_fd;

    struct s_list_cmd // structure of the array of pointers to store commands
    {
        std::string cmd;
        void	(Serveur::*f)(std::string, int);
    };
    struct s_list_cmd _list_cmd[10]; // the array

    std::map<std::string, Channel> _listChannel; // map which contains channels, see the file channel.hpp
	std::vector<std::string> _name_used;
	std::vector<std::string> _register;
	std::string _password;

	int	_serverSocket_fd;
	int _port;
	int _index_connection; // act as switch to differentiate command such as NICK, JOIN, USER from the /connect and the user

	public:
	Serveur(int port, char *password);
	~Serveur();


    void set_list_command();
	void set_port(int pt);
	int get_port();
	void set_password(std::string pass);
	std::string get_password();

	void cmd_User(std::string cmd, int fd_key);
	void print_cmd(std::string cmd, int fd_key);
	void parsing_cmd(Client& client, int fd_key);
	void Make_Sets_Sockets(fd_set& Sets_Sockets);
	void launch_serveur();
	void read_client_message(std::vector<int>& list_Clients_fd, fd_set Sets_Sockets);
	void execute_message(int fd_key, char *message);
	void deconnect_client(std::vector<int>& list_Clients_fd, int i);
	void add_new_connection(int serverSocket_fd, fd_set Sets_Sockets, std::vector<int>& list_Clients_fd);

	void cmd_Nick(std::string cmd, int fd_key);

	void	set_index_connexion(int index); // setter for connection's index
	int		get_index_connexion(); // getter
	void    commun_cmd(std::string cmd, int fd_key); // function which will redirect the string to our command
	void    cmd_tmp(std::string string, int fd_key); // temporary command to fill my array with an address
	void    cmd_join(std::string string, int fd_key); // function Join
	int     verif_name(std::string name); //verification if the channel already exit
//    std::string join_get_error(std::string server, std::string nbr, std::string nick, std::string channel, std::string message); //function to return the numerical error for the clients

};

#endif
