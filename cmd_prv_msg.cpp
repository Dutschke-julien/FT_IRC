//
// Created by PC on 18/10/2023.
//

#include "Serveur.hpp"

void Serveur::cmd_msg(std::string string, int fd_key) {
	string.erase(string.find_last_of('\n'), 1);
	string.erase(string.find_last_of('\r'), 1);

	std::vector<std::string> word;
	std::string tmp;
	std::istringstream iss(string);
	std::string first;
	std::string second;
	std::getline(iss, first, ':');
	std::getline(iss, second);
	std::istringstream split(first);

	if (first.empty()) {
		std::string erreur = ":42Mulhouse 411 " + _mapClients[fd_key].get_nickname()
		                     + " :No recipient given\r\n";
		send(fd_key, erreur.c_str(), erreur.length(), 0);
		return;
	}
	if (second.empty()) {
		std::string erreur = ":42Mulhouse 412 " + _mapClients[fd_key].get_nickname()
		                     + " :No text to send\r\n";
		send(fd_key, erreur.c_str(), erreur.length(), 0);
		return;
	}
	while (split >> tmp) {
		word.push_back(tmp);
	}
	if (word.size() >= 2)
		return;

	if (first.find('#') != std::string::npos) {
		string.erase(string.find_last_of('#'), 1);
		if (verif_name(word[0]) == 0) {
			std::string erreur = ":42Mulhouse 404 " + _mapClients[fd_key].get_nickname()
			                     + " #" + word[0] +  " :Cannot send to channel\r\n";
			send(fd_key, erreur.c_str(), erreur.length(), 0);
			return;
		}
		std::list<int> user = _listChannel[word[0]].get_list_user();
		for (std::list<int>::iterator i = user.begin(); i != user.end() ; i++) {
			std::string reply = ":" + _mapClients[fd_key].get_nickname() 
								+ " PRIVMSG #" + word[0]
								+ " :" + second
			                    + "\r\n";
			if (*i != fd_key)
				send(*i, reply.c_str(), reply.length(), 0);
		}
	} else {
		if (get_fd(word[0]) == -1) {
			std::cout << "nickname not found\n";
			std::string erreur = ":42Mulhouse 401 " + _mapClients[fd_key].get_nickname()
			                     + " " + word[0] +  " :No such nickname\r\n";
			send(fd_key, erreur.c_str(), erreur.length(), 0);
			return;
		}
		std::string reply = ":" + _mapClients[fd_key].get_nickname()
				+ " PRIVMSG " + word[0]
				+ " :" + second
				+ "\r\n";
		send(get_fd(word[0]), reply.c_str(), reply.length(), 0);
	}

}