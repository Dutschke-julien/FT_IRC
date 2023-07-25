#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>

int main() {
	int clientSocket;
	struct sockaddr_in serverAddr;

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);


	int opt = 20;


	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8080);
	serverAddr.sin_addr.s_addr = htons(INADDR_ANY);

	setsockopt(clientSocket, SOL_SOCKET,
			   SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

	if (connect(clientSocket,
				(struct sockaddr *)&serverAddr,
				sizeof(serverAddr)) < 0) {
		perror("Erreur lors de la connexion au serveur");
		close(clientSocket);
		exit(EXIT_FAILURE);
	}

	while (true){
		std::string str;
		std::cout << ">> ";
		std::cin >> str;
		const char *message = str.c_str();
		if (send(clientSocket, message, strlen(message), 0) < 0)
			break;
	}
	close(clientSocket);
	std::cout << "error : break";

//	send(clientSocket, message, strlen(message), 0);

	return 0;
}
