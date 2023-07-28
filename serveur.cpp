#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define MAX_CLIENTS 10 // Nombre maximum de clients pouvant se connecter simultanément

int main() {
    int 	serverSocket, maxSocket, activity, clientSocket, i;
	int clientSockets[MAX_CLIENTS]; 
    
	struct sockaddr_in serverAddr, clientAddr; //structure d'adresse type ipv4 pour client et serveur
	socklen_t clientAddrLen = sizeof(clientAddr);
    
	char buffer[1024];

    
	
	// Créer un socket TCP
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    // Lier le socket à une adresse IP et un port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4242);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

	//bind va lier le fd du socket server avec la structure d'adresse serveur.
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Erreur lors de la liaison du socket");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Mettre le socket en mode écoute
    if (listen(serverSocket, 5) < 0)
	{
        perror("Erreur lors de la mise en écoute du socket");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    printf("En attente de connexions...\n");

    while (1) {
        // Initialiser l'ensemble des sockets (clientSockets) et définir le descripteur maximum
        fd_set readSockets;//enssemble de descripteur utilisable par select()
        FD_ZERO(&readSockets); // met a vide l'ensemble de descripteur
        FD_SET(serverSocket, &readSockets); //ajoute le socket du serveur a l'enssemble

        maxSocket = serverSocket;//select va regarder jusqu'au plus au fd connu dans readSocket ici server Socket

        // Ajouter les sockets des clients actifs à l'ensemble 
        for (i = 0; i < MAX_CLIENTS; i++)
		{
            clientSocket = clientSockets[i];
            if (clientSocket > 0)
			{
                FD_SET(clientSocket, &readSockets);
                if (clientSocket > maxSocket) //mettre a jour maxSocket pour select()
				{
                    maxSocket = clientSocket;
                }
            }
        }


        // Utiliser select() pour attendre l'activité sur les sockets
        activity = select(maxSocket + 1, &readSockets, NULL, NULL, NULL);
        if (activity < 0)
		{
            perror("Erreur lors de l'appel à select");
            close(serverSocket);
            exit(EXIT_FAILURE);
        }



        // Si une nouvelle connexion entrante arrive, accepter la connexion
        if (FD_ISSET(serverSocket, &readSockets)) 
		{
            clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
         
		 
		    if (clientSocket < 0) {
                perror("Erreur lors de l'acceptation de la connexion");
                close(serverSocket);
                exit(EXIT_FAILURE);
            }

            printf("Nouvelle connexion, socket fd : %d, adresse IP : %s, port : %d\n",
                   clientSocket, inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

            // Ajouter le nouveau socket client à l'ensemble
            for (i = 0; i < MAX_CLIENTS; i++)
			{
                if (clientSockets[i] == 0) {
                    clientSockets[i] = clientSocket;
                    break;
                }
            }
        }

        
		
		
		
		// Traiter les données reçues des clients existants
        for (i = 0; i < MAX_CLIENTS; i++) {
            clientSocket = clientSockets[i];

            if (FD_ISSET(clientSocket, &readSockets)) {
                int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
                if (bytesRead <= 0) {
                    // Connexion interrompue ou erreur, fermer le socket et supprimer le client
                    getpeername(clientSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
                    printf("Client déconnecté, adresse IP : %s, port : %d\n",
                           inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

                    close(clientSocket);
                    clientSockets[i] = 0;
                } else {
                    // Afficher les données reçues
                    buffer[bytesRead] = '\0'; // Ajouter le caractère de fin de chaîne
                    printf("Message reçu du client %d : %s\n", clientSocket, buffer);

                    // Répondre au client (facultatif)
                    const char *response = "Message reçu par le serveur !";
                    send(clientSocket, response, strlen(response), 0);
                }
            }
        }
    }

    // Fermer le socket serveur (ce code ne sera jamais atteint dans ce cas)
    close(serverSocket);

    return 0;
}
