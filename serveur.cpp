/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aho <aho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 16:09:02 by aho          #+#    #+#             */
/*   Updated: 2023/10/19 15:49:43 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serveur.hpp"

int Serveur::get_moderator()
{
	return (_moderator);
}

int		Serveur::get_fd(std::string nickname)
{
	if (nickname.empty())
		return (-1);
	
	std::vector<int>::iterator it = _list_Clients_fd.begin();
	while(it != _list_Clients_fd.end())
	{
		if (_mapClients[*it].get_nickname() == nickname)
			return (*it);
		it++;
	}
	return (-1);
}

void	Serveur::cmd_tmp(std::string string, int fd_key)
{
	(void)string;
	(void)fd_key;
}

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
    _first_connection = 0;
    
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
	del_Nick_toks(_register, _mapClients[list_Clients_fd[i]].get_realname());
	del_Nick_toks(_name_used, _mapClients[list_Clients_fd[i]].get_nickname());
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

void Serveur::commun_cmd(std::string cmd, int fd_key) {

    std::string::iterator start_it;
    std::string word;

    if (cmd == "PING 42Mulhouse\r\n") {
        std::string response = "PONG 42Mulhouse\r\n";
        send(fd_key, response.c_str(), response.length(), 0);
    }

    start_it = cmd.begin();
    while (*start_it != ' ' && start_it != cmd.end() && *start_it != '\r' && *start_it != '\n') {
        word += *start_it;
        start_it++;
    }

    if (cmd != "PING 42Mulhouse\r\n")
        std::cout << "le client [" << fd_key << "] a envoyer la cmd suivante : " << cmd << std::endl;
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
	if (_mapClients[fd_key].get_status() == KICK)
	{
		deconnect_client(_list_Clients_fd, _mapClients[fd_key]._vector_index);
		return;
	}	
	std::string input = client.pack.cmd.substr(0, find_command + 2);

        if (input == "PING 42Mulhouse\r\n")
		{
			std::string response = "PONG 42Mulhouse\r\n";
            send(fd_key, response.c_str(), response.length(), 0);
		}
		else if (input == "CAP LS 302\r\n")
		{
			std::string response = "CAP * LS :\r\n";
			send(fd_key, response.c_str(), response.length(), 0);
		}
		else if (input == "CAP END") {
			set_index_connexion(1);
		}
        else
            commun_cmd(input, fd_key);
        client.pack.cmd.erase(0, find_command + 2);
        find_command = client.pack.cmd.find("\r\n");
	}
	std::cout << "la commande :" << client.pack.cmd;
}


void Serveur::launch_serveur()
{
	if (_password.find_first_of(" ") != std::string::npos)
	{
		std::cout << "Erreur format mot de passe\n";
		return;
	}
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

		if (_first_connection == 0)
		{
			_moderator = clientSocket_fd;
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
			_mapClients[clientSocket_fd]._vector_index = i;
                    break;
                }
            }
        }
}

void Serveur::set_list_command() {
	this->_list_cmd[0].cmd = "JOIN";
	this->_list_cmd[0].f = &Serveur::cmd_join;
	this->_list_cmd[1].cmd = "NICK";
    this->_list_cmd[1].f = &Serveur::cmd_Nick;
    this->_list_cmd[2].cmd = "PRIVMSG";
    this->_list_cmd[2].f = &Serveur::cmd_msg;
    this->_list_cmd[3].cmd = "USER";
    this->_list_cmd[3].f = &Serveur::cmd_User;
    this->_list_cmd[4].cmd = "KICK";
    this->_list_cmd[4].f = &Serveur::cmd_kick;
    this->_list_cmd[5].cmd = "PASS";
    this->_list_cmd[5].f = &Serveur::Cmd_Pass;
    this->_list_cmd[6].cmd = "INVITE";
    this->_list_cmd[6].f = &Serveur::cmd_invite;
    this->_list_cmd[7].cmd = "TOPIC";
    this->_list_cmd[7].f = &Serveur::cmd_topic;
    this->_list_cmd[8].cmd = "MODE";
    this->_list_cmd[8].f = &Serveur::cmd_mode;
	this->_list_cmd[9].cmd = "userhost";
    this->_list_cmd[9].f = &Serveur::cmd_User;
}
