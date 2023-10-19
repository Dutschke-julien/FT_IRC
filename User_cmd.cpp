/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 18:02:42 by jdutschk          #+#    #+#             */
/*   Updated: 2023/10/19 15:40:00 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serveur.hpp"


void Serveur::Cmd_Pass(std::string cmd, int fd_key)
{
	cmd.erase(cmd.find_last_not_of("\r\n") + 1);
	std::istringstream iss(cmd);

	std::string mdp;

	iss >> mdp;

	if (mdp == get_password())
	{
		_mapClients[fd_key].set_status(NO_NICK);
		return ;
	}
	else
		_mapClients[fd_key].set_status(KICK);
}


void Serveur::cmd_User(std::string cmd, int fd_key)
{

	std::cout << "Voici la valeur du status :" << _mapClients[fd_key].get_status() << std::endl;
	if (_mapClients[fd_key].get_status() == NO_PASS || _mapClients[fd_key].get_status() == NO_NICK)
	{
		_mapClients[fd_key].set_status(KICK);
		return;
	}
	
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
			username += "_";
			_mapClients[fd_key].set_realname(real_Name);
			_register.push_back(username);
			//std::string bienvenue = ":42Mulhouse 001 " + username + " :Bienvenue sur votre serveur IRC\r\n";
			//send(fd_key, bienvenue.c_str(), bienvenue.length(), 0);
			_mapClients[fd_key].set_status(ALL_OK);
		}
		else
		{
			_mapClients[fd_key].set_realname(real_Name);
			_register.push_back(username);
			//std::string bienvenue = ":42Mulhouse 001 " + username + " :Bienvenue sur votre serveur IRC\r\n";
			//send(fd_key, bienvenue.c_str(), bienvenue.length(), 0);
			_mapClients[fd_key].set_status(ALL_OK);
		}
}

