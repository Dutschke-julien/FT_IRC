/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:30:55 by jdutschk          #+#    #+#             */
/*   Updated: 2023/10/04 17:29:51 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serveur.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
        std::cout << "Usage: " << av[0] << " <port> <password>" << std::endl;
        return 1;
    }

	if (atoi(av[1]) <= 0 || atoi(av[1]) > 65535)
	{
		std::cout <<"Not valid port\n";
		return 1;
	}
	Serveur Serveur(atoi(av[1]), av[2]);
	
    Serveur.launch_serveur();

    return 0;
}