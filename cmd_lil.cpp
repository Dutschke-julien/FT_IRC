//
// Created by PC on 18/10/2023.
//

#include "Serveur.hpp"

void Serveur::cmd_kick(std::string string , int fd_key) {
	string.erase(string.find_last_of('\n'), 1);
	string.erase(string.find_last_of('\r'), 1);
	if (string.find('#') == std::string::npos) {
		std::string error = ":42Mulhouse 461 "
		                    + _mapClients[fd_key].get_nickname()
		                    + "#" + _mapClients[fd_key].get_current_channel() + " KICK "
		                    + " :bad channel mask\r\n";
		send(fd_key, error.c_str(), error.length(), 0);
		return;
	}
	string.erase(string.find_first_of('#'), 1);
	if (string.find('#') != std::string::npos) {
		std::string error = ":42Mulhouse 461 "
		                    + _mapClients[fd_key].get_nickname()
		                    + " INVITE :bad channel mask\r\n";
		send(fd_key, error.c_str(), error.length(), 0);
		return;
	}
	std::istringstream iss(string);
	std::vector<std::string> word;
	std::string tmp;
	while (iss >> tmp) {
		word.push_back(tmp);
	}
	if (!(word.size() == 2 || word.size() == 3)) {
		std::string error = ":42Mulhouse 461 "
		                    + _mapClients[fd_key].get_nickname()
		                    + " KICK :wrong number of arguments to process\r\n";
		send(fd_key, error.c_str(), error.length(), 0);
		return;
	}
	if (verif_name(word[0]) == 0) {
		std::string erreur = ":42Mulhouse 403 " + _mapClients[fd_key].get_nickname()
		                     + " #" + word[1] +  " :No such channel\r\n";
		send(fd_key, erreur.c_str(), erreur.length(), 0);
		return;
	}
	if (_listChannel[word[0]].find_client(fd_key) == 0) {
		std::string erreur = ":42Mulhouse 442 " + _mapClients[fd_key].get_nickname()
		                     + " #" + word[0] +  " :You're not on that channel\r\n";
		send(fd_key, erreur.c_str(), erreur.length(), 0);
		return;
	}
	if (get_fd(word[1]) == -1) {
		return;
	}
	if (_listChannel[word[0]].get_topic_restriction() == -1 && _listChannel[word[0]].find_oper(fd_key) == 0){
		std::string error = ":42Mulhouse 482 "
		                    + _mapClients[fd_key].get_nickname()
		                    + " #" + word[0]
		                    + " :You're not channel operator\r\n";
		send(fd_key, error.c_str(), error.length(), 0);
		return;
	}
	if (_listChannel[word[0]].find_client(get_fd(word[1])) == 0) {
		std::string erreur = ":42Mulhouse 441 " + _mapClients[fd_key].get_nickname()
		                     + " " + word[1]
		                     + " #" + word[0] + " :They aren't on that channel\r\n";
		send(fd_key, erreur.c_str(), erreur.length(), 0);
		return;
	}
	std::string kick = ":42Mulhouse KICK #" + word[0] + " " + word[1];
	if (word.size() == 3 && word[2] != ":")
		kick += " :" + word[2];
	else
		kick += " : No reason was given";
	kick += "\r\n";
	send(get_fd(word[1]), kick.c_str(), kick.length(), 0);
	_listChannel[word[0]].remove_client(get_fd(word[1]));
	std::list<int> user = _listChannel[word[0]].get_list_user();
	for (std::list<int>::iterator i = user.begin(); i != user.end() ; i++) {
		send(*i, kick.c_str(), kick.length(), 0);
	}
}

// SAME FUNCTION

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
		std::string erreur = ":42Mulhouse 401 " + _mapClients[fd_key].get_nickname()
		                     + " " + word[0] +  " :No such nickname\r\n";
		send(fd_key, erreur.c_str(), erreur.length(), 0);
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
