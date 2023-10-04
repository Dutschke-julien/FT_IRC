/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur_cmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 18:02:42 by jdutschk          #+#    #+#             */
/*   Updated: 2023/10/04 17:57:21 by jdutschk         ###   ########.fr       */
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
			std::string erreur = ":server_name 462 * :Vous avez déjà enregistré les informations de l'utilisateur.\r\n";
			send(fd_key, erreur.c_str(), erreur.length(), 0);

		}
		else
		{
			_mapClients[fd_key].set_realname(real_Name);
			_register.push_back(username);
			std::string bienvenue = ":server_name 001 " + username + " :Vous pouvez entrer votre PASS\r\n";
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
			std::string erreur = ":server_name 464 * :Mot de passe incorrect. Veuillez vérifier votre mot de passe.\r\n";
			send(fd_key, erreur.c_str(), erreur.length(), 0);
		}
		else
		{
			_mapClients[fd_key].set_status(2);
			std::string bienvenue = ":server_name 001 * :Bienvenue sur le serveur IRC. Vous êtes maintenant connecté.\r\n";
			send(fd_key, bienvenue.c_str(), bienvenue.length(), 0);

		}
	}
	else
	{
		std::string erreur = ":server_name 462 * :Vous avez déjà enregistré les informations de l'utilisateur.\r\n";
		send(fd_key, erreur.c_str(), erreur.length(), 0);
	}
}

