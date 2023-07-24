/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 17:04:00 by jdutschk          #+#    #+#             */
/*   Updated: 2023/07/24 17:09:08 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    // Créer un socket TCP
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Erreur lors de la création du socket client");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse du serveur
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Adresse IP locale du serveur

    // Connexion au serveur
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Erreur lors de la connexion au serveur");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    // Envoyer un message au serveur
    char *message = "Bonjour, serveur !";
    send(clientSocket, message, strlen(message), 0);

    // Fermer le socket client
    close(clientSocket);

    return 0;
}
