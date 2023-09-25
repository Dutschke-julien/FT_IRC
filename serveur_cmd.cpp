/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur_cmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 18:02:42 by jdutschk          #+#    #+#             */
/*   Updated: 2023/09/19 19:10:29 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serveur.hpp"

void Serveur::cmd_tmp(std::string string, int fd_key) {
    std::cout << string << " fd = " << fd_key << std::endl;
}

void Serveur::cmd_Nick(std::string cmd, int fd_key)
{
    // Supprime les caractères de fin de ligne ("\r\n") de la commande
    cmd.erase(cmd.find_last_not_of("\r\n") + 1);

    // Extrait le pseudonyme de la commande
    std::string pseudonyme = cmd.substr(5);  // Supprime les 5 premiers caractères ("NICK ")

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
        send(fd_key, erreur.c_str(), erreur.length(), 0);
    }
    else
    {
        // Pseudonyme valide, envoie un message de bienvenue
        std::string bienvenue = ":server_name 001 " + pseudonyme + " :Bienvenue sur le serveur IRC\r\n";
        send(fd_key, bienvenue.c_str(), bienvenue.length(), 0);
    }
}

