/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serveur.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 15:19:56 by jdutschk          #+#    #+#             */
/*   Updated: 2023/10/10 17:01:36 by jdutschk         ###   ########.fr       */
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
# include <list>
# include <vector>
# include <unistd.h>
# include <cstring>
# include <sstream>
# include <arpa/inet.h>
# include <netinet/in.h>
# include "client.hpp"
# include "channel.hpp"
# define MAX_CLIENTS 10
# define KICK -1
# define NO_PASS 0
# define NO_NICK 1
# define NO_USERNAME 2
# define ALL_OK 3


bool isNickTaken(const std::vector<std::string>& nameVector, const std::string& newNick);
void del_Nick_toks(std::vector<std::string>& nameVector, std::string Nick);

class Serveur
{
	private:
	
	std::map <int, Client> _mapClients;
	std::vector<int> _list_Clients_fd;

    struct s_list_cmd
    {
        std::string cmd;
        void	(Serveur::*f)(std::string, int);
    };
    struct s_list_cmd _list_cmd[11]; // the array

    std::map<std::string, Channel> _listChannel;
	std::vector<std::string> _name_used;
	std::vector<std::string> _register;
	std::string _password;

	int	_serverSocket_fd;
	int _port;
	int _index_connection; // (obsolete)

	public:
	Serveur(int port, char *password);
	~Serveur();

    	void set_list_command();
	void set_port(int pt);
	int get_port();
	void set_password(std::string pass);
	std::string get_password();

	void Cmd_Pass(std::string cmd, int fd_key);
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

    void    commun_cmd(std::string cmd, int fd_key); // function which will redirect the string to our command
    void    cmd_tmp(std::string string, int fd_key); // temporary command to fill my array with an address
    void    cmd_mode(std::string string, int fd_key); // function mode
    void    cmd_join(std::string string, int fd_key); // function Join
    void    cmd_topic(std::string cmd, int fd_key); // function topic

    void	set_index_connexion(int index); // setter for connection's index (obsolete)
    int		get_index_connexion(); // getter (obsolete)

    void    reply_join(std::string channel, int fd_key); //function which send reply, topic and list to the client
    int     verif_name(std::string name); //verification if the channel already exit

    void    send_topic(std::string channel, int fd_key); //function which send to the client (fd_key) the channel's topic
    int     check_topic(std::string channel, int fd_key); // function with basic verification

    void    modify_topic(std::string channel, std::string topic, int fd_key); //function which will modify and send to all client the topic
};

#endif
