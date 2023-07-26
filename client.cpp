/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 11:07:45 by jdutschk          #+#    #+#             */
/*   Updated: 2023/07/26 18:38:36 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Créer un socket TCP
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Erreur lors de la création du socket client");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse du serveur
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4242);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Adresse IP locale du serveur

    // Connexion au serveur
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Erreur lors de la connexion au serveur");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    // Boucle pour envoyer des phrases en continu
    while (true) {
        std::cout << "Entrez une phrase à envoyer (ou tapez 'exit' pour quitter) : ";
        std::string message;
        std::getline(std::cin, message); // Lire toute la ligne entrée par l'utilisateur

        // Vérifier si l'utilisateur veut quitter
        if (message == "exit")
		{
            break;
        }

        // Envoyer la phrase au serveur
        send(clientSocket, message.c_str(), message.size(), 0);
    }

    // Fermer le socket client
    close(clientSocket);

    return 0;
}
