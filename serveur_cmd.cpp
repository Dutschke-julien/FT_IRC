/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur_cmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 18:02:42 by jdutschk          #+#    #+#             */
/*   Updated: 2023/09/28 17:27:01 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serveur.hpp"




void del_Nick_toks(std::vector<std::string>& nameVector, std::string Nick)
{
	std::vector<std::string>::iterator it = nameVector.begin();

	while (it != nameVector.end())
	{
		if (*it == Nick)
		{
			nameVector.erase(it);
			break;
		}
		else
			it++;
	}
}
bool isNickTaken(const std::vector<std::string>& nameVector, const std::string& newNick) 
{
    for (std::vector<std::string>::const_iterator it = nameVector.begin(); it != nameVector.end(); ++it) 
	{
        if (*it == newNick) 
		{
            return true; // Le nick est déjà pris
        }
    }
    return false; // Le nick n'est pas pris
}

void Serveur::cmd_tmp(std::string string, int fd_key) {
    std::cout << string << " fd = " << fd_key << std::endl;
}

void Serveur::cmd_Nick(std::string cmd, int fd_key)
{
    // Supprime les caractères de fin de ligne ("\r\n") de la commande
	
    std::string pseudonyme = cmd.erase(cmd.find_last_not_of("\r\n") + 1);

	for (std::string::iterator it = pseudonyme.begin(); *it == ' '; it++)
	{
		pseudonyme.erase(it);
	}
    // Vérifie si le pseudonyme est vide
    if (pseudonyme.empty())
    {
        // Pseudonyme manquant, envoie un message d'erreur
        std::string erreur = ":server_name 431 * :Pseudonyme manquant. Utilisation : /NICK <pseudonyme>\r\n";
        send(fd_key, erreur.c_str(), erreur.length(), 0);
    }
    else if (pseudonyme.find_first_of("#@!:%&*,._- ") != std::string::npos)
    {
        // Caractères non autorisés dans le pseudonyme, envoie un message d'erreur
        std::string erreur = ":server_name 432 * :Caractères non autorisés dans le pseudonyme\r\n";
		std::cout << "voici la chaine [" << pseudonyme << "]" << std::endl;
        send(fd_key, erreur.c_str(), erreur.length(), 0);
    }
	else if (isNickTaken(_name_used, pseudonyme) == true)
	{
		std::string erreur = ":server_name 433 * " + pseudonyme + " :Ce pseudonyme est déjà utilisé, veuillez en choisir un autre.\r\n";
    	send(fd_key, erreur.c_str(), erreur.length(), 0);
	}
    else
    {
        // Pseudonyme valide, envoie un message de bienvenue
        std::string bienvenue = ":server_name 001 " + pseudonyme + " :Bienvenue sur le serveur IRC\r\n";
        send(fd_key, bienvenue.c_str(), bienvenue.length(), 0);
		_name_used.push_back(pseudonyme);
		del_Nick_toks(_name_used, _mapClients[fd_key].get_nickname());
		_mapClients[fd_key].set_nickname(pseudonyme);
    }
}
