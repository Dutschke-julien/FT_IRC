//
// Created by PC on 22/09/2023.
//

#include "Serveur.hpp"

int Serveur::verif_name(std::string name) {
	if (!(_listChannel.empty())) {
		if (_listChannel.find(name) != _listChannel.end())
			return 1;
	}
	return 0;
}
void Serveur::set_index_connexion(int index) {
    _index_connection = index;
}

int Serveur::get_index_connexion() {
    return _index_connection;
}
