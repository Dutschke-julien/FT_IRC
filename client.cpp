#include "client.hpp"

void Client::set_current_channel(std::string channel) {
    _current_channel = channel;
}

std::string Client::get_current_channel() {
    return _current_channel;
}

void	Client::add_channel_invitation(std::string channel) {
	_channel_invite.push_back(channel);
}

void	Client::add_channel(std::string channel) {
	_list_channel_join.push_back(channel);
}

void	Client::remove_channel(std::string channel) {
	_list_channel_join.remove(channel);
}

std::list<std::string> Client::get_list_joined_channel() {
	return _list_channel_join;
}

int Client::find_and_remove_invitation(std::string channel) {
	for (std::list<std::string>::iterator i = _channel_invite.begin(); i != _channel_invite.end() ; i++) {
		if (*i == channel) {
			_channel_invite.remove(channel);
			return 1;
		}
	}
	return 0;
}

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