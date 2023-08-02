/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 16:09:02 by jdutschk          #+#    #+#             */
/*   Updated: 2023/08/02 19:04:23 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define MAX_CLIENTS 10
#define PORT 4242


int main()
{
	std::vector<int> listClients(MAX_CLIENTS, 0);
    int serverSocket, clientSocket;	
	int activity;
	sockaddr_in serverAddr, clientAddr;  
    socklen_t clientAddrLen = sizeof(clientAddr);
   	char buffer[1024];

  
  
    // Créer un socket TCP , et renvois le Fd correspondant
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
 
	//gestions erreur d'ouverture de socket
    if (serverSocket < 0)
	{
        perror("Erreur lors de la création du socket serveur\n");
        exit(EXIT_FAILURE);
	}

    // Lier le socket à une adresse IP et un port
    serverAddr.sin_family = AF_INET; //set la famille d'addresse en ipv4
    serverAddr.sin_port = htons(PORT); //convertis le port 4242
    serverAddr.sin_addr.s_addr = INADDR_ANY; //adresse autoriser a se connecter a se socket 
	
	
	//lie le socket a toute les ip machines et au port 4242
	if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) 
	{
        perror("Erreur lors de la liaison du socket serveur avec les infos donner");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }


    // Mettre le socket en mode écoute
    if (listen(serverSocket, 5) < 0) 
	{
        perror("Erreur lors de la mise en écoute du socket serveur\n");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }



    std::cout << "En attente de connexions..." << std::endl;



    while (true) 
	{
     	//creer la plage de socket que select va  analyser
	    fd_set readSockets;
        FD_ZERO(&readSockets);
        FD_SET(serverSocket, &readSockets);


        for (int i = 0; i < MAX_CLIENTS; i++)		
		{
            clientSocket = listClients[i];
			
			//si le fd est superieur a zero , on le rajoute dans readSockets
            if (clientSocket > 0) 
			{
                FD_SET(clientSocket, &readSockets);
            }
        }



        activity = select(FD_SETSIZE, &readSockets, NULL, NULL, NULL);

        if (activity < 0) 
		{
            perror("Erreur lors de l'appel à select()\n");
            close(serverSocket);
            exit(EXIT_FAILURE);
        }

		//regarde si le socket client est pret a lire 
        if (FD_ISSET(serverSocket, &readSockets)) 
		{
			//acccept la nouvelle connection et stocker les infos dans clientAddr
			clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
            
			if (clientSocket < 0) 
			{
                perror("Erreur lors de l'acceptation de la connexion du nouveau client au serveur\n");
                close(serverSocket);
                exit(EXIT_FAILURE);
            }


            std::cout << "Nouvelle connexion, socket fd : " << clientSocket //fd correspondant
                      << ", adresse IP : " << inet_ntoa(clientAddr.sin_addr) //ip correspondant
                      << ", port : " << clientAddr.sin_port << std::endl; // port correspondant


            for (int i = 0; i < MAX_CLIENTS; i++) 
			{
				//ajoute la nouvelle connection sur une case disponible du vecteur
                if (listClients[i] == 0) 
				{
                    listClients[i] = clientSocket;
                    break;
                }
            }
        }
		
		
        for (int i = 0; i < MAX_CLIENTS; i++) 
		{
            clientSocket = listClients[i];
            //regarde si les socket des clients sont pret a lire
			if (FD_ISSET(clientSocket, &readSockets))
			{
				//stock les infos lues
                int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
                
				//libere la place en cas de deconections 
				if (bytesRead <= 0) 
				{
                    getpeername(clientSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
                    std::cout << "Client déconnecté, adresse IP : " << inet_ntoa(clientAddr.sin_addr)
                              << ", port : " << clientAddr.sin_port << std::endl;

                    close(clientSocket);
                    listClients[i] = 0;
                }
		  	
			
				else //affiche le message sur le serveur et envois une reponse au client vis a vis du serveur
				{
                    buffer[bytesRead] = '\0';
                    std::cout << "Message reçu du client " << clientSocket << " : " << buffer << std::endl;

                    const char *response = "Message reçu par le serveur !";
                    send(clientSocket, response, strlen(response), 0);
                }
            }
        }
    }

    close(serverSocket);

    return 0;
}
