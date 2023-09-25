//
// Created by PC on 22/09/2023.
//

#include "Serveur.hpp"

void Serveur::set_index_connexion(int index) {
    _index_connection = index;
}

int Serveur::get_index_connexion() {
    return _index_connection;
}

void Serveur::cmd_join(std::string string, int fd_key) {
	std::string::iterator it = string.begin();
	std::string::iterator pass_it;
	std::string::iterator pass_end_it;
	std::string error;
	std::string channel_name;
	std::string password;
	std::map<std::string, Channel>::iterator channel_it;
	int stat_pass = 0;
	int index_code_error = 0;

	/*
	 * Small portion of code which is looking if the nbr of arguments is not zero
	 * the end of the string should finish by \r\n, but a \r can be found alone
	 * if that's the case, I should remove it to parse easier the rest of the code with .erase
	 */

	while (*it == ' ') {
		string.erase(it);
		it++;
	}
	if (*it == '\r') {
		it++;
		if (*it == '\n') {
            std::cout << "send : not enough parameters\n";
			error = ":yourserver . 461:Not enough parameters\r\n";
			send(fd_key, error.c_str(), error.length(), 0);
			return;
		}
		string.erase(string.begin(), it);
	}
    std::cout << *it << " == it\n";
    std::cout << "entering command JOIN\n" << string << " == string\n";
	if (*it != '#') {
        std::cout << "send : no channel found\n";
		error = ":lol : no channel found\r\n";
		send(fd_key, error.c_str(), error.length(), 0);
		return;
	}

	/*
	 * the rest is cut in two part, first is to find the channel name, if they're one, it should start with #
	 * the second part is figuring out the pass associated with and call the right function in this case
	 */
	while (it != string.end()) {
		/*
		 * there is the first part, because of the preparation up there, the first character I should find is #
		 * then I advance until the first non-alphanumeric character which intend that #first#second is a correct typo
		 */
		while (!(isalnum(*it)) && it != string.end()) {
			it++;
		}
		channel_name = string.substr(0, (std::distance(string.begin(), it) - 1));
		string.erase(string.begin(), it);
		if (!(*it == ' ' || *it == '#') && isprint(*it)) {
            std::cout << "send : bad channel mask\n";
			error = ":yourserver . 403:Bad channel mask\r\n";
			send(fd_key, error.c_str(), error.length(), 0);
			return;
		}

		/*
		 * now I got the channel server, I need to look for the Password by using two new iterator
		 * it should be separated by a space like "#first#second pass"
		 */
		while (it != string.end()) {
			if (*it == ' ' && stat_pass == 0)
				stat_pass = 1;
			else if (*it == '#' && stat_pass == 1)
				stat_pass = 0;
			else if (stat_pass == 1 && *it != ' ') {
				pass_it = it;
				stat_pass = 2;
			} else if (stat_pass == 2 && (*it == ' ' || !(isprint(*it)))) {
				pass_end_it = it;
				password = string.substr((std::distance(string.begin(), pass_it)) - 1,
				                         (std::distance(string.begin(), pass_end_it) - 1));
				string.erase(pass_it, pass_end_it);
				break;
			}
			it++;
		}

		/*
		 * here is where we use the function for the right case
		 */
		channel_it = _listChannel.find(channel_name);
		if (channel_it != _listChannel.end()) {
			if (it != string.end())
				index_code_error = _listChannel[channel_name].verif_pass(password);
			if (index_code_error == -3) {
                std::cout << "send : pass wrong\n";
				error = ":yourserver . 475:Cannot join channel (pass)\r\n";
				send(fd_key, error.c_str(), error.length(), 0);
			} else {
				index_code_error = _listChannel[channel_name].add_client(fd_key);
				switch (index_code_error) {
					case -1:
                        std::cout << "send : banned\n";
						error = ":yourserver . 474:You are banned from this channel\r\n";
						send(fd_key, error.c_str(), error.length(), 0);
						break;
					case -2:
                        std::cout << "send : already registered\n";
						error = ":yourserver . 474:You are already registered in this channel\r\n";
						send(fd_key, error.c_str(), error.length(), 0);
						break;
					default:
                        std::cout << "send : default\n";
						error = ":yourserver . 400:Unexpected error from a /Join command\r\n";
						send(fd_key, error.c_str(), error.length(), 0);
						break;

				}
			}
		} else {
			if (it != string.end())
				_listChannel[channel_name] = Channel(fd_key, password);
			else
				_listChannel[channel_name] = Channel(fd_key);
		}
	}
}
