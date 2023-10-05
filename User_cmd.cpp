/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 18:02:42 by jdutschk          #+#    #+#             */
/*   Updated: 2023/10/05 17:40:36 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serveur.hpp"

void Serveur::cmd_User(std::string cmd, int fd_key)
{
	if(_mapClients[fd_key].get_status() == 0)
	{
		cmd.erase(cmd.find_last_not_of("\r\n") + 1);
		std::istringstream iss(cmd);

		std::string first_part, real_Name, username, hostname, servername;
		
		std::getline(iss, first_part, ';');
		std::getline(iss, real_Name, ';');
		
		iss.clear();
		iss.str(cmd);
		iss >> username >> hostname >> servername;
		
		if (isNickTaken(_register, username))
		{
			std::string erreur = ":42Mulhouse 462 * :Vous etes deja connecter depuis une autre interface\r\n";
			send(fd_key, erreur.c_str(), erreur.length(), 0);

		}
		else
		{
			_mapClients[fd_key].set_realname(real_Name);
			_register.push_back(username);
			std::string bienvenue = ":42Mulhouse 001 " + username + " :Vous pouvez entrer votre PASS\r\n";
			send(fd_key, bienvenue.c_str(), bienvenue.length(), 0);
			_mapClients[fd_key].set_status(1);
		}
	}
		
	
	
	else if (_mapClients[fd_key].get_status() == 1)
	{
		cmd.erase(cmd.find_last_not_of("\r\n") + 1);
		std::istringstream iss(cmd);
		std::string mdp;
		iss >> mdp;
		if (mdp != this->get_password())
		{
			std::string erreur = ":42Mulhouse 464 * :Mot de passe incorrect. Veuillez vérifier votre mot de passe.\r\n";
			send(fd_key, erreur.c_str(), erreur.length(), 0);
		}
		else
		{
			_mapClients[fd_key].set_status(2);
			std::string bienvenue = ":42Mulhouse 001 * :Bienvenue sur le serveur IRC. Vous êtes maintenant connecté.\r\n";
			send(fd_key, bienvenue.c_str(), bienvenue.length(), 0);

		}
	}


	
	else
	{
		std::cout << "la valeur du status " <<  _mapClients[fd_key].get_status() << std::endl;
		std::string erreur = ":42Mulhouse 462 * :Vous etes deja entierement enregistré\r\n";
		send(fd_key, erreur.c_str(), erreur.length(), 0);
	}
}

