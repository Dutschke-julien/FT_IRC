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

Serveur::cmd_Nick(std::string cmd, int fd_key)
{
//	std::string name = cmd.erase(0, 5);
    
    if (name.find_first_of("#@!:%&*,._- ") != std::string::npos)
        std::cout << "Error Wrong caract found\n"; // Error a changer
    else if ((name.length() - 2) > 9 )
        std::cout << "Error too much caract only 9\n"; //Error a changer
	
	std::vector<std::string>::iterator it = _name_used.begin();
	
	while (it != _name_used.end())
	{
		if (it == name || name == _name_used.end())
			std::cout << "Error Nick already taken\n";
		it++;
	}
	_name_used.push_back(name);
	_list_Client_fd[fd_key].set_nickname(name);
	
	//message de validation a envoyer au clients 
}
