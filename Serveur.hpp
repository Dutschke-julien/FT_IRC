/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serveur.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 15:19:56 by jdutschk          #+#    #+#             */
/*   Updated: 2023/09/12 16:26:53 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVEUR_HPP
# define SERVEUR_HPP

# include <stdio.h>
# include <iostream>
# include <stdlib.h>
# include <string>
# include <map>
# include <vector>
# include <unistd.h>
# include <cstring>
# include <arpa/inet.h>
# include <netinet/in.h>
# include "client.hpp"
# define MAX_CLIENTS 10


class Serveur
{
	private:
	
	std::map <int, Client> _mapClients;
	std::vector<int> _list_Clients_fd;
	
	std::string _password;

	int	_serverSocket_fd;
	int _port;

	public:
	Serveur(int port, char *password);
	~Serveur();

	void set_port(int pt);
	int get_port();
	void set_password(std::string pass);
	std::string get_password();
	
	
	void Make_Sets_Sockets(fd_set& Sets_Sockets);
	void processCommands(std::string& clientData, int fd_key_client);
	void launch_serveur();
	void read_client_message(std::vector<int>& list_Clients_fd, fd_set Sets_Sockets);
	void execute_message(int fd_key, char *message);
	void deconnect_client(std::vector<int>& list_Clients_fd, int i);
	void add_new_connection(int serverSocket_fd, fd_set Sets_Sockets, std::vector<int>& list_Clients_fd);
};



#endif
