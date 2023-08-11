/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 15:19:56 by jdutschk          #+#    #+#             */
/*   Updated: 2023/08/11 17:28:28 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVEUR_HPP
# define SERVEUR_HPP

# include <iostream>
# include <string>
# include <vector>
# include <unistd.h>
# include <cstring>
# include <arpa/inet.h>
# include <netinet/in.h>

# define MAX_CLIENTS 10
# define PORT 4242

class Serveur
{
	private:
	int	serverSocket_fd;
	std::vector<int> list_Clients_fd;
	
	public:
	Serveur();
	~Serveur();
	
	void launch_serveur();
	void read_client_message(std::vector<int>& list_Clients_fd, fd_set Sets_Sockets);
	void print_message_client(const std::vector<int>& list_Clients_fd, int i, char *message);
	void deconnect_client(std::vector<int>& list_Clients_fd, int i);
	void add_new_connection(int serverSocket_fd, fd_set Sets_Sockets, std::vector<int>& list_Clients_fd);
};



#endif
