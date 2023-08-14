/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 16:09:02 by jdutschk          #+#    #+#             */
/*   Updated: 2023/08/11 17:02:36 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serveur.hpp"

Serveur::~Serveur()
{
	std::cout << "destructeur call\n";
	close(serverSocket_fd);
}

Serveur::Serveur()
{
    sockaddr_in serverAddr;
    int         serverSocket_fd;
    
	std::cout << "constructor call\n";
    serverSocket_fd = socket(AF_INET, SOCK_STREAM, 0);
	list_Clients_fd.resize(MAX_CLIENTS, 0);

    if (serverSocket_fd < 0)
	{
        perror("Erreur lors de la création du socket serveur\n");
        exit(EXIT_FAILURE);
	}

    serverAddr.sin_family = AF_INET; //set la famille d'addresse en ipv4
    serverAddr.sin_port = htons(PORT); //convertis le port 4242
    serverAddr.sin_addr.s_addr = INADDR_ANY; //adresse autoriser a se connecter a se socket 
	
	
	//lie le socket a toute les ip machines et au port 4242
	if (bind(serverSocket_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) 
	{
        perror("Erreur lors de la liaison du socket serveur avec les infos donner");
        close(serverSocket_fd);
        exit(EXIT_FAILURE);
    }


    // Mettre le socket en mode écoute
    if (listen(serverSocket_fd, 5) < 0) 
	{
        perror("Erreur lors de la mise en écoute du socket serveur\n");
        close(serverSocket_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "En attente de connexions sur le serveur..." << std::endl;

	this->serverSocket_fd = serverSocket_fd;
}



void Make_Sets_Sockets(int serverSocket_fd, const std::vector<int>& list_Clients_fd, fd_set& Sets_Sockets)
{
    FD_ZERO(&Sets_Sockets);
    FD_SET(serverSocket_fd, &Sets_Sockets);

    for (int i = 0; i < MAX_CLIENTS; i++)		
		{
			//si le fd est superieur a zero , on le rajoute dans Sets_Sockets
            if (list_Clients_fd[i] > 0) 
			{
                FD_SET(list_Clients_fd[i], &Sets_Sockets);
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
                    break;
                }
            }
        }
}



void Serveur::deconnect_client(std::vector<int>& list_Clients_fd, int i)
{
	sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	
	getpeername(list_Clients_fd[i], (struct sockaddr *)&clientAddr, &clientAddrLen);
                   
	std::cout << "Client déconnecté, adresse IP : " << inet_ntoa(clientAddr.sin_addr)<< ", port : " << clientAddr.sin_port << std::endl;
                  
	close(list_Clients_fd[i]);
                  
	list_Clients_fd[i] = 0;
}

void Serveur::print_message_client(const std::vector<int>& list_Clients_fd, int i, char *message)
{
	
	std::cout << "Message reçu du client " << list_Clients_fd[i] << " : " << message << std::endl;
	const char *response = "Message reçu par le serveur !";
	
	send(list_Clients_fd[i], response, strlen(response), 0);
}

void Serveur::read_client_message(std::vector<int>& list_Clients_fd, fd_set Sets_Sockets)
{
	char message[1024];

	
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
					print_message_client(list_Clients_fd, i, message);
                }
            }
        }
}



void Serveur::launch_serveur()
{
   	  	
     while (true) 
	{
     	//creer la plage de socket que select va  analyser
        fd_set Sets_Sockets;
        Make_Sets_Sockets(serverSocket_fd, list_Clients_fd, Sets_Sockets);

        if (select(FD_SETSIZE, &Sets_Sockets, NULL, NULL, NULL) == -1) 
		{
            perror("Erreur lors de l'appel à select()\n");
            close(serverSocket_fd);
            exit(EXIT_FAILURE);
        }

		add_new_connection(serverSocket_fd, Sets_Sockets, list_Clients_fd);
		read_client_message(list_Clients_fd, Sets_Sockets);
    }
}

int main()
{
	Serveur Serveur;
  
    Serveur.launch_serveur();

    return 0;
}
