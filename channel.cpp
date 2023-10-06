#include "channel.hpp"

Channel::Channel() {}

Channel::~Channel() {}

Channel::Channel(int fd_oper) {
    _invite_only = 0;
    _topic_restriction = 0;
    _password_set = 0;
    _limit_client = 0;
	_oper.push_back(fd_oper);
	_full_list.push_back(fd_oper);
}

Channel::Channel(int fd_oper, std::string pass) {
	_oper.push_back(fd_oper);
	_full_list.push_back(fd_oper);
	_pass = pass;
    _invite_only = 0;
    _topic_restriction = 0;
    _password_set = 1;
    _limit_client = 0;
}

int Channel::find_client(int fd_key) {
    for (std::list<int>::iterator i = _full_list.begin(); i != _full_list.end() ; i++) {
        if (*i == fd_key)
            return (1);
    }
    return (0);
}

int Channel::find_oper(int fd_key) {
    for (std::list<int>::iterator i = _oper.begin(); i != _oper.end() ; i++) {
        if (*i == fd_key)
            return (1);
    }
    return (0);
}

std::list<int> Channel::get_list_user() {
    return _full_list;
}

int Channel::get_topic_restriction() {
    return _topic_restriction;
}

void Channel::set_topic(std::string topic) {
    _topic = topic;
}

std::string Channel::get_topic() {
    return _topic;
}

int Channel::add_client(int fd_key) {
    std::cout << "entering add_client function\n";
	for (std::list<int>::const_iterator it_find = _ban.begin();
	it_find != _ban.end(); it_find++) {
		if (*it_find == fd_key) {
            std::cout << "exit add_client1 function\n";
            return -1;
        }
	}
	for (std::list<int>::const_iterator it = _full_list.begin();
	     it != _full_list.end(); it++) {
		if (*it == fd_key) {
            std::cout << "exit add_client2 function\n";
            return -2;
        }
	}
	_full_list.push_back(fd_key);
    std::cout << "exit add_client3 function\n";
	return 0;
}

int Channel::add_client(int fd_key, std::string pass) {
    std::cout << "entering add_client_pass function\n";
	if (_pass.empty()) {
        add_client(fd_key);
        std::cout << "exiting add_client_pass2 function\n";
        return 2;
        }
    else {
        if (pass != _pass) {
            std::cout << "exiting add_client_pass1 function\n";
            return (-3);
		}
	}
	return (add_client(fd_key));
}

std::string Channel::get_pass() {
    return _pass;
}

int Channel::add_client_ban() {
	return 0;
}

int Channel::remove_client() {
	return 0;
}
