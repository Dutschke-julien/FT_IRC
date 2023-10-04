/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utlis_function.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 17:55:42 by jdutschk          #+#    #+#             */
/*   Updated: 2023/10/04 18:07:38 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serveur.hpp"

void del_Nick_toks(std::vector<std::string>& nameVector, std::string Nick)
{
	std::vector<std::string>::iterator it = nameVector.begin();

	while (it != nameVector.end())
	{
		if (*it == Nick)
		{
			nameVector.erase(it);
			break;
		}
		else
			it++;
	}
}
bool isNickTaken(const std::vector<std::string>& nameVector, const std::string& newNick) 
{
    for (std::vector<std::string>::const_iterator it = nameVector.begin(); it != nameVector.end(); ++it) 
	{
        if (*it == newNick) 
		{
            return true; // Le nick est déjà pris
        }
    }
    return false; // Le nick n'est pas pris
}
