/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdutschk <jdutschk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:27:37 by jdutschk          #+#    #+#             */
/*   Updated: 2023/07/24 11:30:51 by jdutschk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP

# define IRC_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>

class Irc
{
	private : 
	int 			_port;
	std::string		_password;

	public :
	Irc();
	~Irc();

};

#endif