/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 17:03:50 by jdutschk          #+#    #+#             */
/*   Updated: 2023/07/24 17:17:09 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buffer[1024];

    // Créer un socket TCP
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
	{
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }



    // Lier le socket à une adresse IP et un port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); //Convertis	 le 8080 en valeur binaire
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    

	if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
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

    // Attente de connexions entrantes
    printf("En attente de connexions...\n");

    // Accepter la connexion entrante
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (clientSocket < 0) {
        perror("Erreur lors de l'acceptation de la connexion");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Lire les données du client et afficher
    recv(clientSocket, buffer, sizeof(buffer), 0);
    printf("Message reçu du client : %s\n", buffer);

    // Fermer les sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
