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
	if (!(_listChannel.empty())) {
		if (_listChannel.find(name) != _listChannel.end()) {
            return 1;
        }
	}
	return 0;
}
void Serveur::set_index_connexion(int index) {
    _index_connection = index;
}

int Serveur::get_index_connexion() {
    return _index_connection;
}