//
// Created by PC on 22/09/2023.
//

#include "Serveur.hpp"

//std::string Serveur::join_get_error(std::string server, std::string nbr, std::string nick, std::string channel,
//                                    std::string message) {
//    std::string result;
//    result
//}

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
