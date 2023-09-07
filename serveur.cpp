/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 16:09:02 by jdutschk          #+#    #+#             */
/*   Updated: 2023/08/24 18:13:30 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serveur.hpp"

void Serveur::set_port(int pt)
{
	port = pt;
}

int Serveur::get_port()
{
	return (port);
}

void Serveur::set_password(std::string pass)
{
	password = pass;
}

std::string Serveur::get_password()
{
	return(password);
}

Serveur::~Serveur()
{
	std::cout << "destructeur call\n";
	close(serverSocket_fd);
}

Serveur::Serveur(int port)
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
    serverAddr.sin_port = htons(port); //convertis le port 4242
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



void Serveur::Make_Sets_Sockets(int serverSocket_fd, const std::vector<int>& list_Clients_fd, fd_set& Sets_Sockets)
{
    FD_ZERO(&Sets_Sockets);
    FD_SET(serverSocket_fd, &Sets_Sockets);

    for (int i = 0; i < MAX_CLIENTS; i++)		
		{	
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
					mapClients[clientSocket_fd].clientAddr = clientAddr;
					mapClients[clientSocket_fd].status = NO_PWD;
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
	
	mapClients.erase(list_Clients_fd[i]);
	close(list_Clients_fd[i]);
	list_Clients_fd[i] = 0;
}

void Serveur::execute_message(int fd_key, char *message)
{
	std::string input = message;
	
	if (mapClients[fd_key].status == NO_PWD)
	{
		if (input != password)
		{
			std::cout << "le client donne [" + input + "]" << "et le mdp original [" + password + "]";
			send(fd_key, "Wrong password\nTry again\n", 25, 0);
		}
		else
		{
			mapClients[fd_key].status = NO_NKN;
			send(fd_key, "password ok\nchoose a nickname\n", 30, 0);
		}
	}
	else if (mapClients[fd_key].status == NO_NKN)
	{
		if (input.find("/Nickname") == 0)
		{
			mapClients[fd_key].name = input.substr(10);
			send(fd_key, "Nickname set\n", 13, 0);
		}
		else
			send(fd_key, "Please choose a Nickname first with /Nickname\n", 46, 0);
	}
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
					execute_message(list_Clients_fd[i], message);
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
int ft_strlen(char *str){
	while(str[i])
		i++;
	return i;
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
        std::cout << "Usage: " << av[0] << " <port> <password>" << std::endl;
        return 1;
    	}
	if (ft_strlen(av[2]) > 8)
	{
		std::cout << "[Error] password too long!\n";
		return (1);
	}
	if (atoi(av[1]) <= 0 || atoi(av[1]) > 65535)
	{
		std::cout <<"Not valid port\n";
		return 1;
	}
	Serveur Serveur(atoi(av[1]));
	Serveur.set_password(av[2]);
    Serveur.launch_serveur();

    return 0;
}
