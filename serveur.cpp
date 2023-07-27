/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 11:07:42 by jdutschk          #+#    #+#             */
/*   Updated: 2023/07/26 18:40:52 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

//la Configuration du serveur a l'heure actuel ne nous permet pas de mettre
//en place l'ecoute sur plusieurs clients , fact : 

/*recv() est une fonction bloquante par défaut lorsqu'elle est utilisée avec des sockets
il est important de noter que vous pouvez configurer le socket
en mode non bloquant en utilisant la fonction fcntl() pour le rendre non bloquant,
ou utiliser l'option SO_RCVTIMEO pour définir un délai de temporisation pour recv().
Dans ces cas, recv() ne bloquera pas indéfiniment
et renverra immédiatement si aucune donnée n'est disponible après le délai spécifié.
configurés en mode bloquant.*/

int main() 
{
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buffer[1024];

    // Créer un socket TCP
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    
	
	if (serverSocket < 0) //gestion d'erreurs creation du socket serveur
		{
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    	}

    // Lier le socket à une adresse IP et un port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4242);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    
	if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		 
        perror("Erreur lors de la liaison du socket");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    
	// Mettre le socket en mode écoute
    if (listen(serverSocket, 5) < 0) {
        perror("Erreur lors de la mise en écoute du socket");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Boucle principale du serveur
    while (true)
	{
        std::cout << "En attente de connexions..." << std::endl;
        
		//bloque le processus tant que la connection n'est pas etablie
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
       
	   
	    if (clientSocket < 0)
		{
            perror("Erreur lors de l'acceptation de la connexion");
            close(serverSocket);
            exit(EXIT_FAILURE);
        }



		
			// Gérer la communication avec le client
			while (true)
			{
				// Recevoir les données du client
				int bytesRead = recv(clientSocket, &buffer, sizeof(buffer), 0);
				if (bytesRead <= 0)
				{
					// Connexion interrompue ou erreur
					break;
				}

				// Afficher les données reçues
				buffer[bytesRead] = '\0'; // Ajouter le caractère de fin de chaîne
				std::cout << "Message reçu du client : " << buffer << std::endl;


				// Répondre au client (facultatif)
				const std::string response = "Message reçu par le serveur !";
				send(clientSocket, response.c_str(), response.size(), 0); //c_str convertir un string en char * (C_style)

			}

		// Fermer la connexion avec le client actuelle
		close(clientSocket);
	}

    // Fermer le socket serveur
    close(serverSocket);

    return 0;
}
