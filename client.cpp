#include "client.hpp"

int Client::get_status(void)
{
	return (_status);
}

void Client::set_status(int status)
{
	_status = status;
}

std::string Client::get_username(void)
{
	return (_username);
}

void Client::set_username(std::string nick)
{
	_username = nick;
}

std::string Client::get_realname(void)
{
	return (_realname);
}


void Client::set_realname(std::string nick)
{
	_realname = nick;
}


std::string Client::get_nickname(void)
{
	return (_nickname);
}


void Client::set_nickname(std::string nick)
{
	_nickname = nick;
}

sockaddr_in Client::get_clientAddr(void)
{
	return (_clientAddr);
}

void Client::set_clientAddr(sockaddr_in new_struct)
{
	_clientAddr = new_struct;	
}

Client::Client()
{
	std::cout << "Class Client build\n";
	_status = 0;
}

Client::~Client()
{
	std::cout << "Class Client destroy\n";
}