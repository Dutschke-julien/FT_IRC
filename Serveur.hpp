/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serveur.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 15:19:56 by jdutschk          #+#    #+#             */
/*   Updated: 2023/08/28 13:14:56 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVEUR_HPP
# define SERVEUR_HPP

# include <iostream>
# include <string>
#include <map>
# include <vector>
# include <unistd.h>
# include <cstring>
# include <arpa/inet.h>
# include <netinet/in.h>

# define MAX_CLIENTS 10

#define NO_PWD -2
#define NO_NKN -1
#define ALL_OK 0

struct ClientInfo
{
	std::string nickname;
	std::string username;
    	sockaddr_in clientAddr;
	vector<string>	c_channels;
	int status;
};

class Serveur
{
	private:
	vector<string>	s_channels;
	int	serverSocket_fd;
	std::map <int, ClientInfo> mapClients;
	std::vector<int> list_Clients_fd;
	std::string password;
	int port;

	public:
	Serveur(int port);
	~Serveur();

	void set_port(int pt);
	int get_port();
	void set_password(std::string pass);
	std::string get_password();
	void Make_Sets_Sockets(int serverSocket_fd, const std::vector<int>& list_Clients_fd, fd_set& Sets_Sockets);
	void launch_serveur();
	void read_client_message(std::vector<int>& list_Clients_fd, fd_set Sets_Sockets);
	void execute_message(int fd_key, char *message);
	void deconnect_client(std::vector<int>& list_Clients_fd, int i);
	void add_new_connection(int serverSocket_fd, fd_set Sets_Sockets, std::vector<int>& list_Clients_fd);
};



#endif
