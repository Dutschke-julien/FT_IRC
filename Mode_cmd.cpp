#include "Serveur.hpp"

void Serveur::cmd_mode(std::string string, int fd_key) {
    if (string.find('#') == string.npos)
        return;

    string.erase(string.find_last_of('\n'), 1);
    string.erase(string.find_last_of('\r'), 1);
    string.erase(string.find_first_of('#'), 1);
    std::istringstream iss(string);
    std::vector<std::string> word;
    std::string tmp;

    while (iss >> tmp) {
        word.push_back(tmp);
    }

    if (word.size() < 2)
        return;

    if (verif_name(word[0]) == 0) {
        std::string erreur = ":42Mulhouse 403 " + _mapClients[fd_key].get_nickname()
				+ " #" + word[0] +  " :No such channel\r\n";
        send(fd_key, erreur.c_str(), erreur.length(), 0);
        return;
    }
	if (_listChannel[word[0]].find_client(fd_key) == 0) {
		std::string error = ":42Mulhouse 442 "
				+ _mapClients[fd_key].get_nickname()
				+ " #" + word[0]
				+ " :You're not on that channel\r\n";
		send(fd_key, error.c_str(), error.length(), 0);
		return;
	}
    if (word[1] == "+i" && word.size() == 2) {
        _listChannel[word[0]].set_mode_invite(-1);
    } else if (word[1] == "+t") {
		_listChannel[word[0]].set_mode_topic(-1);
    } else if (word[1] == "+k" && word.size() == 3) {
		_listChannel[word[0]].set_password(word[2], 1);
    } else if (word[1] == "+o" && word.size() == 3
			   && _listChannel[word[0]].find_client(get_fd(word[2]))) {
		_listChannel[word[0]].add_oper(get_fd(word[2]));
	} else if (word[1] == "+l" && word.size() == 3) {
		_listChannel[word[0]].set_limit(word[2]);
    } else if (word[1] == "-i"
				&& word.size() == 2) {
		_listChannel[word[0]].set_mode_invite(1);
	} else if (word[1] == "-t") {
		_listChannel[word[0]].set_mode_topic(1);
	} else if (word[1] == "-k") {
		_listChannel[word[0]].set_password("", 0);
	} else if (word[1] == "-o" && word.size() == 3
				&& _listChannel[word[0]].find_client(get_fd(word[2]))) {
		_listChannel[word[0]].remove_oper(get_fd(word[2]));
	} else if (word[1] == "-l") {
		_listChannel[word[0]].set_limit("0");
	} else {
		std::string error = ":42Mulhouse 400 " + _mapClients[fd_key].get_nickname()
				+ " MODE :too much arguments to proccess\r\n";
		send(fd_key, error.c_str(), error.length(), 0);
		return;
	}
	std::list<int> user = _listChannel[word[0]].get_list_user();
	for (std::list<int>::iterator i = user.begin(); i != user.end() ; i++) {
		std::string reply = ":" + _mapClients[fd_key].get_nickname()
							+ " MODE"
							+ " #" + word[0]
							+ " " + word[1];
		if (word.size() == 3)
			reply += " " + word[2];
		reply += "\r\n";
		send(*i, reply.c_str(), reply.length(), 0);
	}
}