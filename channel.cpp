#include "channel.hpp"

/*
 * 1 == all good
 * -1 == Ban
 * -2 == already registered
 * -3 == bad_password
 */


Channel::Channel() {}

Channel::~Channel() {}

Channel::Channel(int fd_oper) {
	_mode = 1;
	_oper.push_back(fd_oper);
	_full_list.push_back(fd_oper);
}

Channel::Channel(int fd_oper, std::string pass) {
	_mode = 1;
	_oper.push_back(fd_oper);
	_full_list.push_back(fd_oper);
	_pass = pass;
}

int Channel::set_mode() {
	return 0;
}

int Channel::add_client(int fd_key) {
	for (std::list<int>::const_iterator it_find = _ban.begin();
	it_find != _ban.end(); it_find++) {
		if (*it_find == fd_key)
			return -1;
	}
	for (std::list<int>::const_iterator it = _full_list.begin();
	     it != _full_list.end(); it++) {
		if (*it == fd_key)
			return -2;
	}
	_full_list.push_back(fd_key);
	return 0;
}

int Channel::add_client(int fd_key, std::string pass) {
	if (!(_pass.empty())) {
		if (pass != _pass)
			return (-3);
		else {
			add_client(fd_key);
			return 2;
		}
	}
	return (add_client(fd_key));
}

int Channel::add_client_ban() {
	return 0;
}

int Channel::remove_client() {
	return 0;
}
