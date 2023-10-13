/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_cmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 16:55:49 by jdutschk          #+#    #+#             */
/*   Updated: 2023/10/10 16:55:50 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serveur.hpp"

int Serveur::verif_name(std::string name) {
    std::cout << "entering verif_name function\n";
	if (!(_listChannel.empty())) {
		if (_listChannel.find(name) != _listChannel.end()) {
            std::cout << "exiting verif_name1 function\n";
            return 1;
        }
	}
    std::cout << "exiting verif_name2 function\n";
	return 0;
}
void Serveur::set_index_connexion(int index) {
    _index_connection = index;
}

int Serveur::get_index_connexion() {
    return _index_connection;
}

void    Serveur::cmd_invite(std::string string, int fd_key) {
	string.erase(string.find_last_of('\n'), 1);
	string.erase(string.find_last_of('\r'), 1);
	if (string.find('#') == std::string::npos) {
		std::string error = ":42Mulhouse 461 "
		                    + _mapClients[fd_key].get_nickname()
		                    + "#" + _mapClients[fd_key].get_current_channel() + " INVITE "
							+ " : bad channel mask\r\n";
		send(fd_key, error.c_str(), error.length(), 0);
		return;
	}
	string.erase(string.find_first_of('#'), 1);
	if (string.find('#') != std::string::npos) {
		std::string error = ":42Mulhouse 461 "
							+ _mapClients[fd_key].get_nickname()
							+ " INVITE : bad channel mask\r\n";
		send(fd_key, error.c_str(), error.length(), 0);
		return;
	}
	std::istringstream iss(string);
	std::vector<std::string> word;
	std::string tmp;

	while (iss >> tmp) {
		word.push_back(tmp);
	}
	if (word.size() != 2) {
		std::string error = ":42Mulhouse 461 "
				+ _mapClients[fd_key].get_nickname()
				+ " INVITE :wrong number of arguments to process\r\n";
		send(fd_key, error.c_str(), error.length(), 0);
		return;
	}
	if (verif_name(word[1]) == 0) {
		std::string erreur = ":42Mulhouse 403 " + _mapClients[fd_key].get_nickname()
		                     + " #" + word[1] +  " :No such channel\r\n";
		send(fd_key, erreur.c_str(), erreur.length(), 0);
		return;
	}
	if (_listChannel[word[1]].find_client(fd_key) == 0) {
		std::string erreur = ":42Mulhouse 442 " + _mapClients[fd_key].get_nickname()
							 + " #" + word[1] +  " :You're not on that channel\r\n";
		send(fd_key, erreur.c_str(), erreur.length(), 0);
		return;
	}
	if (get_fd(word[0]) == -1) {
		return;
	}
	if (_listChannel[word[1]].find_client(get_fd(word[0])) == 1) {
		std::string erreur = ":42Mulhouse 443 " + _mapClients[fd_key].get_nickname()
							+ " " + word[0]
							+ " #" + word[1] + " :is already on channel\r\n";
		send(fd_key, erreur.c_str(), erreur.length(), 0);
		return;
	}
	if (_listChannel[word[1]].get_invite_only() == -1
			&& _listChannel[word[1]].find_oper(fd_key) == 0) {
		std::string error = ":42Mulhouse 482 "
		                    + _mapClients[fd_key].get_nickname()
							+ " #" + word[1]
		                    + " :You're not channel operator\r\n";
		send(fd_key, error.c_str(), error.length(), 0);
		return;
	}
	std::string reply = ":" + _mapClients[fd_key].get_nickname()
			+ " INVITE " + word[0]
			+ " #" + word[1] + "\r\n";
	send(get_fd(word[0]), reply.c_str(), reply.length(), 0);
	_mapClients[get_fd(word[0])].add_channel_invitation(word[1]);
	reply = ":42Mulhouse 341 "
						+ _mapClients[fd_key].get_nickname()
						+ " " + word[0]
						+ " #" + word[1] + "\r\n";
	send(fd_key, reply.c_str(), reply.length(), 0);
}