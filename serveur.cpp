/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 16:09:02 by jdutschk          #+#    #+#             */
/*   Updated: 2023/09/19 17:46:29 by jdutschk         ###   ########.fr       */
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
					_mapClients[list_Clients_fd[i]].pack.cmd += message;
					parsing_cmd(_mapClients[list_Clients_fd[i]], list_Clients_fd[i]);
                }
            }
        }
}


void Serveur::print_cmd(std::string cmd, int fd_key)
{
    if (cmd != "PING yourserver\r\n") {
        std::cout << "le client [" << fd_key << "] a envoyer la cmd suivante : " << cmd << std::endl;
    }
	if (cmd == "CAP LS 302\r\n")
	{
		std::string response = "CAP * LS :\r\n";
		send(fd_key, response.c_str(), response.length(), 0);
	}
	if (cmd == "NICK aho\r\n")
	{
		std::string response = ":yourserver 001 aho :Welcome to the IRC Server aho!@localhost\r\n";
		send(fd_key, response.c_str(), response.length(), 0);
	}
	if (cmd == "USER aho aho localhost :aho aho\r\n")
	{
		std::string response = ":yourserver 002 aho :Your host is yourserver, running version 1.0\r\n";
		send(fd_key, response.c_str(), response.length(), 0);
        set_index_connexion(1);
	}
	if (cmd == "PING yourserver\r\n")
	{
		std::string response = "PONG yourserver\r\n";
		send(fd_key, response.c_str(), response.length(), 0);
	}
}

void Serveur::commun_cmd(std::string cmd, int fd_key) {

    std::string::iterator start_it;
    std::string word;


    if (cmd == "PING yourserver\r\n")
    {
        std::string response = "PONG yourserver\r\n";
        send(fd_key, response.c_str(), response.length(), 0);
    }
    start_it = cmd.begin();
    while (*start_it != ' ' && start_it != cmd.end() && *start_it != '\r' && *start_it != '\n') {
        word += *start_it;
        start_it++;
    }

    /*
     * portion of code to know what the difference between word and the command
    */

    for (std::string::iterator verif = word.begin(); verif != word.end() ; verif++) {
        std::cout << *verif << "-"<< std::endl;
    }
    std::cout << "end of word\n";

    for (std::string::iterator verif = _list_cmd[0].cmd.begin(); verif != _list_cmd[0].cmd.end() ; verif++) {
        std::cout << *verif << "-"<< std::endl;
    }
    std::cout << "end of command\n";

    std::cout << word << " == " << _list_cmd[0].cmd << std::endl;
    if (word == _list_cmd[0].cmd)
        std::cout << "HERE\n";

    /*
     * verification's end
     */

    for (int i = 0; i < 10; ++i) {
        if (word == _list_cmd[i].cmd) {
            cmd.erase(cmd.begin(), start_it);
            (this->*_list_cmd[i].f)(cmd, fd_key);
        }
    }
}

void Serveur::parsing_cmd(Client& client, int fd_key)
{
	int find_command = 0;
	 
	find_command = client.pack.cmd.find("\r\n");

	while (find_command != -1)
	{
        if (get_index_connexion() == 0)
            print_cmd(client.pack.cmd.substr(0, find_command + 2), fd_key);
        else
            commun_cmd(client.pack.cmd.substr(0, find_command + 2), fd_key);
        client.pack.cmd.erase(0, find_command + 2);
        find_command = client.pack.cmd.find("\r\n");
	}
}


void Serveur::launch_serveur()
{
	this->set_list_command();
    this->set_index_connexion(0);
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

void Serveur::set_list_command() {
	this->_list_cmd[0].cmd = "JOIN";
	this->_list_cmd[0].f = &Serveur::cmd_join;
	this->_list_cmd[1].cmd = "NICK";
    this->_list_cmd[1].f = &Serveur::cmd_tmp;
    this->_list_cmd[2].cmd = "PRVMSG";
    this->_list_cmd[2].f = &Serveur::cmd_tmp;
    this->_list_cmd[3].cmd = "USER";
    this->_list_cmd[3].f = &Serveur::cmd_tmp;
    this->_list_cmd[4].cmd = "KICK";
    this->_list_cmd[4].f = &Serveur::cmd_tmp;
    this->_list_cmd[5].cmd = "PASS";
    this->_list_cmd[5].f = &Serveur::cmd_tmp;
    this->_list_cmd[6].cmd = "INVITE";
    this->_list_cmd[6].f = &Serveur::cmd_tmp;
    this->_list_cmd[7].cmd = "TOPIC";
    this->_list_cmd[7].f = &Serveur::cmd_tmp;
    this->_list_cmd[8].cmd = "MODE";
    this->_list_cmd[8].f = &Serveur::cmd_tmp;
    this->_list_cmd[9].cmd = "OPER";
    this->_list_cmd[9].f = &Serveur::cmd_tmp;
}
