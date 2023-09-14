/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 16:09:02 by jdutschk          #+#    #+#             */
/*   Updated: 2023/09/13 16:58:17 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serveur.hpp"


void Serveur::set_port(int pt)
{
	_port = pt;
}

int Serveur::get_port()
{
	return (_port);
}

void Serveur::set_password(std::string pass)
{
	_password = pass;
}

std::string Serveur::get_password()
{
	return(_password);
}


Serveur::Serveur(int port, char *password)
{
    sockaddr_in serverAddr;
    
	std::cout << "constructor call\n";
    _serverSocket_fd = socket(AF_INET, SOCK_STREAM, 0);
	_list_Clients_fd.resize(MAX_CLIENTS, 0);

    if (_serverSocket_fd < 0)
	{
        perror("Erreur lors de la création du socket serveur\n");
        exit(EXIT_FAILURE);
	}
	
	_password = password;
    serverAddr.sin_family = AF_INET; //set la famille d'addresse en ipv4
    serverAddr.sin_port = htons(port); //convertis le port
    serverAddr.sin_addr.s_addr = INADDR_ANY; //adresse autoriser a se connecter a se socket 
	
	
	//lie le socket a toute les ip machines au port 4242
	if (bind(_serverSocket_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) 
	{
        perror("Erreur lors de la liaison du socket serveur avec les infos donner");
        close(_serverSocket_fd);
        exit(EXIT_FAILURE);
    }


    // Mettre le socket en mode écoute
    if (listen(_serverSocket_fd, 5) < 0) 
	{
        perror("Erreur lors de la mise en écoute du socket serveur\n");
        close(_serverSocket_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "En attente de connexions sur le serveur..." << std::endl;
}

Serveur::~Serveur()
{
	std::cout << "destructeur call\n";
	close(_serverSocket_fd);
}








void Serveur::deconnect_client(std::vector<int>& list_Clients_fd, int i)
{
	sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	
	getpeername(list_Clients_fd[i], (struct sockaddr *)&clientAddr, &clientAddrLen);
                   
	std::cout << "Client déconnecté, adresse IP : " << inet_ntoa(clientAddr.sin_addr)<< ", port : " << clientAddr.sin_port << std::endl;
	
	_mapClients.erase(list_Clients_fd[i]);
	close(list_Clients_fd[i]);
	list_Clients_fd[i] = 0;
}



void Serveur::read_client_message(std::vector<int>& list_Clients_fd, fd_set Sets_Sockets)
{
	char message[1024];
	static std::map<int, std::string> clientData;


	for (int i = 0; i < MAX_CLIENTS; i++) 
		{
            //regarde si le socket  est pret a lire d'apres select 
			if (FD_ISSET(list_Clients_fd[i], &Sets_Sockets))
			{
				//stock les infos lues
            	int bytesRead = recv(list_Clients_fd[i], message, sizeof(message), 0);
				
				//libere la place en cas de deconnections 
				if (bytesRead <= 0)
					deconnect_client(list_Clients_fd, i);
				else //affiche le message sur le serveur et envois une reponse au client vis a vis du serveur
				{
            		message[bytesRead] = '\0';
					clientData[list_Clients_fd[i]] += message;
					processCommands(clientData[list_Clients_fd[i]], list_Clients_fd[i]);
                }
            }
        }
}

void Serveur::processCommands(std::string& clientData, int fd_key_client)
{
 		//code pour executer les messages complet dans execute_message
	(void)clientData;
	(void)fd_key_client;
}



void Serveur::launch_serveur()
{
     while (true) 
	{
        fd_set Sets_Sockets;
        Make_Sets_Sockets(Sets_Sockets);

        if (select(FD_SETSIZE, &Sets_Sockets, NULL, NULL, NULL) == -1) 
		{
            perror("Erreur lors de l'appel à select()\n");
            close(_serverSocket_fd);
            exit(EXIT_FAILURE);
        }

		add_new_connection(_serverSocket_fd, Sets_Sockets, _list_Clients_fd);
		read_client_message(_list_Clients_fd, Sets_Sockets);
    }
}



void Serveur::Make_Sets_Sockets(fd_set& Sets_Sockets)
{
    FD_ZERO(&Sets_Sockets);
    FD_SET(_serverSocket_fd, &Sets_Sockets);

    for (int i = 0; i < MAX_CLIENTS; i++)		
		{	
            if (_list_Clients_fd[i] > 0) 
			{
                FD_SET(_list_Clients_fd[i], &Sets_Sockets);
            }
        }
}

void Serveur::add_new_connection(int serverSocket_fd, fd_set Sets_Sockets, std::vector<int>& list_Clients_fd)
{
	
	int clientSocket_fd;
	sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);

	//regarde si le socket serveur est pret pour la lecture`
        if (FD_ISSET(serverSocket_fd, &Sets_Sockets)) 
		{
			//acccept la nouvelle connection et stocker les infos dans clientAddr
			clientSocket_fd = accept(serverSocket_fd, (struct sockaddr *)&clientAddr, &clientAddrLen);
			if (clientSocket_fd < 0) 
			{
                perror("Erreur lors de l'acceptation de la connexion du nouveau client au serveur\n");
                close(serverSocket_fd);
                exit(EXIT_FAILURE);
            }


            std::cout << "Nouvelle connexion, socket fd : " << clientSocket_fd //fd correspondant
                      << ", adresse IP : " << inet_ntoa(clientAddr.sin_addr) //ip correspondant
                      << ", port : " << clientAddr.sin_port << std::endl; // port correspondant


            for (int i = 0; i < MAX_CLIENTS; i++) 
			{
				//ajoute la nouvelle connection sur une case disponible du vecteur
                if (list_Clients_fd[i] == 0) 
				{
                    list_Clients_fd[i] = clientSocket_fd;
					_mapClients[clientSocket_fd].set_clientAddr(clientAddr);
					_mapClients[clientSocket_fd].set_status(NO_PWD);
                    break;
                }
            }
        }
}